#include <string.h>
#include <stdlib.h>

#include "CryptID_ABE.h"
#include "elliptic/TatePairing.h"
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

    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, p, 1);

    mpz_t alpha;
    mpz_init(alpha);
    random_mpzInRange(alpha, pMinusOne);

    mpz_t beta;
    mpz_init(beta);
    random_mpzInRange(beta, pMinusOne);

    publickey->ellipticCurve = ec;
    publickey->g = pointP;

    status = AFFINE_MULTIPLY_IMPL(&publickey->h, beta, publickey->g, publickey->ellipticCurve);
    if(status) {
        // TO DO clears
        return status;
    }

    mpz_t beta_inverse;
    mpz_init(beta_inverse);
    mpz_invert(beta_inverse, beta, one);

    status = AFFINE_MULTIPLY_IMPL(&publickey->f, beta_inverse, publickey->g, publickey->ellipticCurve);
    if(status) {
        // TO DO clears
        return status;
    }

    masterkey->pubkey = publickey;
    mpz_init(masterkey->beta);
    mpz_set(masterkey->beta, beta);

    status = AFFINE_MULTIPLY_IMPL(&masterkey->g_alpha, alpha, publickey->g, publickey->ellipticCurve);
    if(status) {
        // TO DO clears
        return status;
    }

    mpz_clears(zero, one, NULL);
    mpz_clears(p, q, r, pMinusOne, alpha, beta, beta_inverse, NULL);


    status = tate_performPairing(&publickey->pairValue, 2, ec, q, pointP, pointP);
    if(status)
    {
        return status;
    }

    return CRYPTID_SUCCESS;
}

CryptidStatus compute_tree(AccessTree* accessTree, mpz_t s, PublicKey_ABE* publickey)
{
    int d = accessTree->value - 1; // dx = kx-1, degree = treshold-1

    Polynom* q = createPolynom(d, s, publickey);
    if(!isLeaf(accessTree))
    {
        int i;
        for(i = 1; i < MAX_CHILDREN; i++)
        {
            if(accessTree->children[i] != NULL)
            {
                mpz_t sum;
                mpz_init(sum);
                polynomSum(q, i, sum);
                compute_tree(accessTree->children[i], sum, publickey);
                mpz_clear(sum);
            }
        }
    }
    else
    {
        // TO DO
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

    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

    mpz_t s;
    mpz_init(s);
    random_mpzInRange(s, pMinusOne);
    compute_tree(accessTree, s, publickey);

    mpz_clears(pMinusOne, s, NULL);

    return CRYPTID_SUCCESS;
}

/*CryptidStatus cryptid_keygen_ABE(MasterKey_ABE* masterkey, char** attributes, SecretKey_ABE* secretkey)
{
    mpz_init(secretkey->D);

    return CRYPTID_SUCCESS;
}*/