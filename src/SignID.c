#include <string.h>
#include <stdlib.h>

#include "SignID.h"
#include "elliptic/TatePairing.h"
#include "util/Random.h"
#include "util/RandBytes.h"
#include "util/Utils.h"
#include "util/Validation.h"

static const unsigned int SOLINAS_GENERATION_ATTEMPT_LIMIT = 100;
static const unsigned int POINT_GENERATION_ATTEMPT_LIMIT = 100;

static const unsigned int Q_LENGTH_MAPPING[] = { 160, 224, 256, 384, 512 };
static const unsigned int P_LENGTH_MAPPING[] = { 512, 1024, 1536, 3840, 7680 };

SignidStatus signid_setup(const SecurityLevel securityLevel, PublicParameters* publicParameters, mpz_t masterSecret)
{
    if (!publicParameters)
    {
        return SIGNID_PUBLIC_PARAMETERS_NULL_ERROR;
    }

    // Construct the elliptic curve and its subgroup of interest
    // Select a random n_q-bit Solinas prime q
    mpz_t q;
    mpz_init(q);

    SignidStatus status = random_solinasPrime(q, Q_LENGTH_MAPPING[(int) securityLevel], SOLINAS_GENERATION_ATTEMPT_LIMIT);

    if (status)
    {
        mpz_clear(q);
        return status;
    }

    // Select a random integer r, such that p = 12 * r * q - 1 is an n_p-bit prime
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

    mpz_clears(zero, one, NULL);

    // Select a point P of order q in E(F_p)
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

        status = affine_wNAFMultiply(&pointP, rMul, pointPprime, ec);

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

    // Determine the master secret
    mpz_t qMinusTwo, s;
    mpz_init(s);
    mpz_init_set(qMinusTwo, q);
    mpz_sub_ui(qMinusTwo, qMinusTwo, 2);

    random_mpzInRange(s, qMinusTwo);
    mpz_add_ui(s, s, 2);
    mpz_clear(qMinusTwo);

    // Determine the public parameters
    AffinePoint pointPpublic;

    status = affine_wNAFMultiply(&pointPpublic, s, pointP, ec);

    if (status)
    {
        mpz_clears(p, q, s, NULL);
        affine_destroy(pointP);
        ellipticCurve_destroy(ec);
        return status;
    }

    publicParameters->ellipticCurve = ec;
    mpz_set(publicParameters->q, q);
    publicParameters->pointP = pointP;
    publicParameters->pointPpublic = pointPpublic;
    publicParameters->hashFunction = hashFunction_initForSecurityLevel(securityLevel);

    mpz_set(masterSecret, s);

    mpz_clears(p, q, s, r, NULL);

    return SIGNID_SUCCESS;
}

SignidStatus signid_extract(AffinePoint* result, const char *const identity, const size_t identityLength,
                    const PublicParameters publicParameters, const mpz_t masterSecret)
{
    if (!result)
    {
        return SIGNID_RESULT_POINTER_NULL_ERROR;
    }

    if (identityLength == 0)
    {
        return SIGNID_IDENTITY_LENGTH_ERROR;
    }

    if(!validation_isPublicParametersValid(publicParameters))
    {
        return SIGNID_ILLEGAL_PUBLIC_PARAMETERS_ERROR;
    }

    AffinePoint qId;

    SignidStatus status =
        hashToPoint(&qId, publicParameters.ellipticCurve, publicParameters.ellipticCurve.fieldOrder, publicParameters.q, identity, identityLength, publicParameters.hashFunction);

    if (status) 
    {
        return status;
    }

    status = affine_wNAFMultiply(result, masterSecret, qId, publicParameters.ellipticCurve);

    affine_destroy(qId);

    return status;
}

SignidStatus signid_sign(Signature *result, const AffinePoint privateKey, const char *const message, const size_t messageLength,
                    const char *const identity, const size_t identityLength, const PublicParameters publicParameters)
{
    if(!message)
    {
        return SIGNID_MESSAGE_NULL_ERROR;
    }

    if(messageLength == 0)
    {
        return SIGNID_MESSAGE_LENGTH_ERROR;
    }

    if(!identity)
    {
        return SIGNID_IDENTITY_NULL_ERROR;
    }

    if(identityLength == 0)
    {
        return SIGNID_IDENTITY_LENGTH_ERROR;
    }

    if(!validation_isPublicParametersValid(publicParameters))
    {
        return SIGNID_ILLEGAL_PUBLIC_PARAMETERS_ERROR;
    }

    mpz_t k;
    mpz_init(k);
    random_mpzInRange(k, publicParameters.q);

    int hashLen = publicParameters.hashFunction.hashLength;

    AffinePoint pointQId;
    SignidStatus status = hashToPoint(&pointQId, publicParameters.ellipticCurve, publicParameters.ellipticCurve.fieldOrder, publicParameters.q, identity, identityLength, publicParameters.hashFunction);
    if(status)
    {
        return status;
    }
    
    Complex theta;
    status = tate_performPairing(&theta, 2, publicParameters.ellipticCurve, publicParameters.q, pointQId, publicParameters.pointP);
    /*if(status)
    {
        mpz_clear(l);
        affine_destroy(pointQId);
        free(rho);
        free(t);
        free(concat);
        affine_destroy(cipherPointU);
        return status;
    }*/

    Complex thetaPrime = complex_modPow(theta, k, publicParameters.ellipticCurve.fieldOrder);

    int zLength;
    unsigned char* z = canonical(&zLength, publicParameters.ellipticCurve.fieldOrder, thetaPrime, 1);

    unsigned char* w = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    (*(publicParameters.hashFunction.sha_hash))(z, zLength, w);

    unsigned char* t = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    (*(publicParameters.hashFunction.sha_hash))((unsigned char*) message, messageLength, t);

    unsigned char* concat = (unsigned char*)calloc(2*hashLen, sizeof(unsigned char));
    for(int i = 0; i < hashLen; i++)
    {
        concat[i] = w[i];
    }
    for(int i = 0; i < hashLen; i++)
    {
        concat[hashLen + i] = t[i];
    }

    mpz_t cipherV;
    mpz_init(cipherV);
    hashToRange(cipherV, concat, 2*hashLen, publicParameters.q, publicParameters.hashFunction);

    AffinePoint cipherPointU, kMulPointQId, vMulPrivateKey;
    status = affine_wNAFMultiply(&vMulPrivateKey, cipherV, privateKey, publicParameters.ellipticCurve);
    status = affine_wNAFMultiply(&kMulPointQId, k, pointQId, publicParameters.ellipticCurve);
    status = affine_add(&cipherPointU, vMulPrivateKey, kMulPointQId, publicParameters.ellipticCurve);
    /*if(status)
    {
        mpz_clear(l);
        affine_destroy(pointQId);
        free(rho);
        free(t);
        free(concat);
        return status;
    }*/

    *result = signature_init(cipherPointU, cipherV);

    /*mpz_clear(l);
    affine_destroy(pointQId);
    affine_destroy(cipherPointU);
    complex_destroyMany(2, theta, thetaPrime);
    free(rho);
    free(concat);
    free(z);
    free(cipherV);
    free(cipherW);
    free(hashedBytes);*/

    return SIGNID_SUCCESS;
}

SignidStatus signid_verify(const char *const message, const size_t messageLength, const Signature signature,
                    const char *const identity, const size_t identityLength, const PublicParameters publicParameters)
{
    if(!validation_isPublicParametersValid(publicParameters))
    {
        return SIGNID_ILLEGAL_PUBLIC_PARAMETERS_ERROR;
    }

    /*if(!validation_isAffinePointValid(privateKey, publicParameters.ellipticCurve.fieldOrder))
    {
        return ILLEGAL_PRIVATE_KEY_ERROR;
    }
    if(!validation_isCipherTextTupleValid(ciphertext, publicParameters.ellipticCurve.fieldOrder))
    {
        return ILLEGAL_CIPHERTEXT_TUPLE_ERROR;
    }*/

    /*mpz_t l;
    mpz_init(l);*/
    SignidStatus status;

    int hashLen = publicParameters.hashFunction.hashLength;

    Complex theta1;
    status = tate_performPairing(&theta1, 2, publicParameters.ellipticCurve, publicParameters.q, signature.cipherU, publicParameters.pointP);
    if(status)
    {
        return status;
    }

    AffinePoint pointQId;
    status = hashToPoint(&pointQId, publicParameters.ellipticCurve, publicParameters.ellipticCurve.fieldOrder, publicParameters.q, identity, identityLength, publicParameters.hashFunction);
    if(status)
    {
        return status;
    }

    AffinePoint negativePointPpublic;
    mpz_t yNegate, yNegateModP;
    mpz_inits(yNegate, yNegateModP, NULL);
    mpz_neg(yNegate, publicParameters.pointPpublic.y);
    mpz_mod(yNegateModP, yNegate, publicParameters.ellipticCurve.fieldOrder);

    negativePointPpublic = affine_init(publicParameters.pointPpublic.x, yNegateModP);

    Complex theta2;
    status = tate_performPairing(&theta2, 2, publicParameters.ellipticCurve, publicParameters.q, pointQId, negativePointPpublic);
    if(status)
    {
        return status;
    }

    Complex theta2Prime = complex_modPow(theta2, signature.cipherV, publicParameters.ellipticCurve.fieldOrder);

    Complex r = complex_modMul(theta1, theta2Prime, publicParameters.ellipticCurve.fieldOrder);

    int zLength;
    unsigned char* z = canonical(&zLength, publicParameters.ellipticCurve.fieldOrder, r, 1);

    unsigned char* w = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    (*(publicParameters.hashFunction.sha_hash))(z, zLength, w);

    unsigned char* t = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    (*(publicParameters.hashFunction.sha_hash))((unsigned char*) message, messageLength, t);

    unsigned char* concat = (unsigned char*)calloc(2*hashLen + 1, sizeof(unsigned char));
    for(int i = 0; i < hashLen; i++)
    {
        concat[i] = w[i];
    }
    for(int i = 0; i < hashLen; i++)
    {
        concat[hashLen + i] = t[i];
    }
    concat[2*hashLen] = '\0';

    mpz_t v;
    mpz_init(v);
    hashToRange(v, concat, 2*hashLen, publicParameters.q, publicParameters.hashFunction);

    if(mpz_cmp(signature.cipherV, v) == 0)
    {
        //affine_destroy(testPoint);
        //mpz_clear(l);
        //*result = m;
        return SIGNID_SUCCESS;
    }

    //affine_destroy(testPoint);
    //mpz_clear(l);
    return SIGNID_VERIFICATION_FAILED_ERROR;
}