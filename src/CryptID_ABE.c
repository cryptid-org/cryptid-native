#include <string.h>
#include <stdlib.h>

#include "CryptID_ABE.h"
#include "elliptic/TatePairing.h"
#include "util/Random.h"
#include "util/RandBytes.h"
#include "util/Utils.h"
#include "util/Validation.h"



// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


static const unsigned int SOLINAS_GENERATION_ATTEMPT_LIMIT = 100;
static const unsigned int POINT_GENERATION_ATTEMPT_LIMIT = 100;

static const unsigned int Q_LENGTH_MAPPING[] = { 160, 224, 256, 384, 512 };
static const unsigned int P_LENGTH_MAPPING[] = { 512, 1024, 1536, 3840, 7680 };

CryptidStatus cryptid_setup_ABE(const SecurityLevel securityLevel, PublicKey_ABE* publickey, MasterKey_ABE* masterkey)
{
    // Construct the elliptic curve and its subgroup of interest
    // Select a random \f$n_q\f$-bit Solinas prime \f$q\f$.
    mpz_t q;
    mpz_init(q);

    CryptidStatus status = random_solinasPrime(q, Q_LENGTH_MAPPING[(int) securityLevel], SOLINAS_GENERATION_ATTEMPT_LIMIT);

    if (status)
    {
        mpz_clear(q);
        return status;
    }

    // Select a random integer \f$r\f$, such that \f$p = 12 \cdot r \cdot q - 1\f$ is an \f$n_p\f$-bit prime.
    unsigned int lengthOfR = P_LENGTH_MAPPING[(int) securityLevel] - Q_LENGTH_MAPPING[(int) securityLevel] - 3;
    mpz_t r, p;
    mpz_inits(r, p, NULL);
    do
    {
        random_mpzOfLength(r, lengthOfR);
        mpz_mul_ui(p, r, 12);
        mpz_mul(p, p, q);
        mpz_sub_ui(p, p, 1);
    }
    while (!validation_isProbablePrime(p));

    mpz_t zero, one;
    mpz_init_set_ui(zero, 0);
    mpz_init_set_ui(one, 1);

    EllipticCurve ec = ellipticCurve_init(zero, one, p);


    // Select a point \f$P\f$ of order \f$q\f$ in \f$E(F_p)\f$.
    AffinePoint pointP;
    do
    {
        AffinePoint pointPprime;
        status = random_affinePoint(&pointPprime, ec, POINT_GENERATION_ATTEMPT_LIMIT);

        if (status)
        {
            mpz_clears(p, q, r, NULL);
            ellipticCurve_destroy(ec);
            return status;
        }

        mpz_t rMul;
        mpz_init_set(rMul, r);
        mpz_mul_ui(rMul, rMul, 12);

        status = AFFINE_MULTIPLY_IMPL(&pointP, rMul, pointPprime, ec);

        if (status)
        {
            mpz_clears(p, q, r, rMul, NULL);
            ellipticCurve_destroy(ec);
            affine_destroy(pointPprime);
            return status;
        }

        mpz_clear(rMul);
        affine_destroy(pointPprime);
    }
    while (affine_isInfinity(pointP));

    // Select a point \f$P\f$ of order \f$q\f$ in \f$E(F_p)\f$. 2.
    AffinePoint pointP2;
    do
    {
        AffinePoint pointPprime;
        status = random_affinePoint(&pointPprime, ec, POINT_GENERATION_ATTEMPT_LIMIT);

        if (status)
        {
            mpz_clears(p, q, r, NULL);
            ellipticCurve_destroy(ec);
            return status;
        }

        mpz_t rMul;
        mpz_init_set(rMul, r);
        mpz_mul_ui(rMul, rMul, 12);

        status = AFFINE_MULTIPLY_IMPL(&pointP2, rMul, pointPprime, ec);

        if (status)
        {
            mpz_clears(p, q, r, rMul, NULL);
            ellipticCurve_destroy(ec);
            affine_destroy(pointPprime);
            return status;
        }

        mpz_clear(rMul);
        affine_destroy(pointPprime);
    }
    while (affine_isInfinity(pointP2));

    /*Complex theta;
    status = tate_performPairing(&theta, 2, ec, q, pointP, pointP2);
    if(status)
    {
        return status;
    }*/

    mpz_t alpha;
    mpz_init(alpha);
    random_mpzOfLength(alpha, p-1);

    mpz_t beta;
    mpz_init(beta);
    random_mpzOfLength(beta, p-1);


    mpz_t beta_inverse;
    mpz_init(beta_inverse);
    mpz_invert(beta_inverse, beta, one);

    publickey->g = q;
    mpz_init(publickey->h);
    mpz_pow_ui(publickey->h, g, beta);
    mpz_init(publickey->f);
    mpz_pow_ui(publickey->f, g, beta_inverse);

    masterkey->pubkey = publickey;
    masterkey->beta = beta;
    mpz_init(masterkey->galpha);
    mpz_pow_ui(masterkey->galpha, g, alpha);

    mpz_clears(zero, one, NULL);
    mpz_clears(p, q, r, alpha, beta, beta_inverse, NULL);

    /*Complex theta;
    status = tate_performPairing(&theta, 2, ec, q, pointP, pointP2);
    if(status)
    {
        return status;
    }*/
    // TO DO: publickey->eggalpha = pairing

    return CRYPTID_SUCCESS;
}
