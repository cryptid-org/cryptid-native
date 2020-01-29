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

CryptidStatus randomGroupElement(mpz_t element, mpz_t p, mpz_t g)
{
    mpz_t randn;
    mpz_init(randn);
    mpz_init_set_ui(randn, 0);
    while(mpz_cmp_ui(randn, 1) < 0) {
        random_mpzInRange(randn, p);
    }
    
    mpz_powm(element, g, randn, p);

    return CRYPTID_SUCCESS;
}

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

    mpz_t alpha;
    mpz_init(alpha);
    randomGroupElement(alpha, p, q);

    mpz_t beta;
    mpz_init(beta);
    randomGroupElement(beta, p, q);

    mpz_t beta_inverse;
    mpz_init(beta_inverse);
    mpz_invert(beta_inverse, beta, one);

    mpz_init(publickey->g);
    mpz_set(publickey->g, q);
    mpz_init(publickey->h);
    mpz_pow_ui(publickey->h, publickey->g, mpz_get_ui(beta));
    mpz_init(publickey->f);
    mpz_pow_ui(publickey->f, publickey->g, mpz_get_ui(beta_inverse));

    masterkey->pubkey = publickey;
    mpz_init(masterkey->beta);
    mpz_set(masterkey->beta, beta);
    mpz_init(masterkey->galpha);
    mpz_pow_ui(masterkey->galpha, publickey->g, mpz_get_ui(alpha));

    mpz_clears(zero, one, NULL);
    mpz_clears(p, q, r, alpha, beta, beta_inverse, NULL);

    AffinePoint pointPpublic;
    status = AFFINE_MULTIPLY_IMPL(&pointPpublic, publickey->g, pointP, ec);
    if(status) {
        // TO DO clears
        return status;
    }

    Complex theta;
    status = tate_performPairing(&theta, 2, ec, q, pointPpublic, pointPpublic);
    if(status)
    {
        return status;
    }

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_encrypt_ABE(const char *const message, const size_t messageLength,
                                    PublicKey_ABE* publickey, AccessTree* accessTree)
{
    if(!message)
    {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(messageLength == 0)
    {
        return CRYPTID_MESSAGE_LENGTH_ERROR;
    }

    if(!publickey) {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(!accessTree) {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    //compute_tree(accessTree, s, publickey);

    return CRYPTID_SUCCESS;
}