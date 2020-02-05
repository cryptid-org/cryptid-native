#include <string.h>
#include <stdlib.h>

#include "SignID.h"
#include "elliptic/TatePairing.h"
#include "util/Random.h"
#include "util/RandBytes.h"
#include "util/Utils.h"
#include "util/Validation.h"

// References
//  * [HESS-IBS] Florian Hess. 2003. Efficient Identity Based Signature Schemes Based on Pairings.
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


static const unsigned int SOLINAS_GENERATION_ATTEMPT_LIMIT = 100;
static const unsigned int POINT_GENERATION_ATTEMPT_LIMIT = 100;

static const unsigned int Q_LENGTH_MAPPING[] = { 160, 224, 256, 384, 512 };
static const unsigned int P_LENGTH_MAPPING[] = { 512, 1024, 1536, 3840, 7680 };

CryptidStatus cryptid_ibs_hess_setup(const SecurityLevel securityLevel, PublicParameters* publicParameters, mpz_t masterSecret)
{
    // Implementation of Algorithm 5.1.2 (BFsetup1) in [RFC-5091].
    // Note, that instead of taking the bitlengts of p and q as arguments, this function takes
    // a security level which is in turn translated to bitlengths using {@code P_LENGTH_MAPPING} and
    // {@code Q_LENGTH_MAPPING}.

    if (!publicParameters)
    {
        return CRYPTID_PUBLIC_PARAMETERS_NULL_ERROR;
    }

    // Construct the elliptic curve and its subgroup of interest
    // Select a random n_q-bit Solinas prime q
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

    mpz_clears(zero, one, NULL);

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
    // TODO: Also init to better conform with other functions.
    mpz_set(publicParameters->q, q);
    publicParameters->pointP = pointP;
    publicParameters->pointPpublic = pointPpublic;
    publicParameters->hashFunction = hashFunction_initForSecurityLevel(securityLevel);

    mpz_set(masterSecret, s);

    mpz_clears(p, q, s, r, NULL);

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_ibs_hess_extract(AffinePoint* result, const char *const identity, const size_t identityLength,
                    const PublicParameters publicParameters, const mpz_t masterSecret)
{
    // Implementation of Algorithm 5.3.1 (BFextractPriv) in [RFC-5091].

    if (!result)
    {
        return CRYPTID_RESULT_POINTER_NULL_ERROR;
    }

    if (identityLength == 0)
    {
        return CRYPTID_IDENTITY_LENGTH_ERROR;
    }

    if(!validation_isPublicParametersValid(publicParameters))
    {
        return CRYPTID_ILLEGAL_PUBLIC_PARAMETERS_ERROR;
    }

    AffinePoint qId;

    // Let \f$Q_{id} = \mathrm{HashToPoint}(E, p, q, id, \mathrm{hashfcn})\f$.
    CryptidStatus status =
        hashToPoint(&qId, publicParameters.ellipticCurve, publicParameters.ellipticCurve.fieldOrder, publicParameters.q, identity, identityLength, publicParameters.hashFunction);

    if (status) 
    {
        return status;
    }

    // Let \f$S_{id} = [s]Q_{id}\f$.
    status = affine_wNAFMultiply(result, masterSecret, qId, publicParameters.ellipticCurve);

    affine_destroy(qId);

    return status;
}

CryptidStatus cryptid_ibs_hess_sign(Signature *result, const AffinePoint privateKey, const char *const message, const size_t messageLength,
                    const char *const identity, const size_t identityLength, const PublicParameters publicParameters)
{
    // Implementation of Scheme 1. Sign in [HESS-IBS].

    if(!message)
    {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(messageLength == 0)
    {
        return CRYPTID_MESSAGE_LENGTH_ERROR;
    }

    if(!identity)
    {
        return CRYPTID_IDENTITY_NULL_ERROR;
    }

    if(identityLength == 0)
    {
        return CRYPTID_IDENTITY_LENGTH_ERROR;
    }

    if(!validation_isPublicParametersValid(publicParameters))
    {
        return CRYPTID_ILLEGAL_PUBLIC_PARAMETERS_ERROR;
    }

    mpz_t k;
    mpz_init(k);
    // Let (@code k) be a random number in range of (@code publicParameters.q).
    random_mpzInRange(k, publicParameters.q);

    // Let {@code hashlen} be the length of the output of the cryptographic hash
    // function hashfcn from the public parameters.
    int hashLen = hashFunction_getHashSize(publicParameters.hashFunction);

    // \f$Q_{id} = \mathrm{HashToPoint}(E, p, q, id, \mathrm{hashfcn})\f$
    // which results in a point of order \f$q\f$ in \f$E(F_p)\f$.
    AffinePoint pointQId;
    CryptidStatus status = hashToPoint(&pointQId, publicParameters.ellipticCurve, publicParameters.ellipticCurve.fieldOrder, publicParameters.q, identity, identityLength, publicParameters.hashFunction);
    if(status)
    {
        mpz_clear(k);
        return status;
    }
    
    // Let \f$\mathrm{theta} = \mathrm{Pairing}(E, p, q, Q_{id}, P_{pub})\f$, which is an element of
    // the extension field \f$F_p^2\f$ obtained using the modified Tate pairing.
    Complex theta;
    status = tate_performPairing(&theta, 2, publicParameters.ellipticCurve, publicParameters.q, pointQId, publicParameters.pointP);
    if(status)
    {
        mpz_clear(k);
        affine_destroy(pointQId);
        return status;
    }

    // Let \f$\mathrm{r} = \mathrm{theta}^k\f$, which is theta raised to the power of \f$k\f$ in \f$F_p^2\f$.
    Complex r = complex_modPow(theta, k, publicParameters.ellipticCurve.fieldOrder);

    // Let \f$z = \mathrm{Canonical}(p, k, 0, \mathrm{r})\f$, a canonical string
    // representation of {@code r}.
    int zLength;
    unsigned char* z = canonical(&zLength, publicParameters.ellipticCurve.fieldOrder, r, 1);

    // Let \f$w = \mathrm{hashfcn}(z)\f$ using the {@code hashfcn} hashing algorithm, the
    // result of which is a {@code hashlen}-octet string.
    unsigned char* w = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    hashFunction_hash(publicParameters.hashFunction, z, zLength, w);

    // Let \f$t = \mathrm{hashfcn}(message)\f$ using the \f$hashfcn\f$ algorithm.
    unsigned char* t = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    hashFunction_hash(publicParameters.hashFunction, (unsigned char*) message, messageLength, t);

    // Let \f$v = \mathrm{HashToRange}(w || t, q, \mathrm{hashfcn}) using HashToRange
    // on the \f$(2 \cdot \mathrm{hashlen})\f$-octet concatenation of {@code w} and
    // \f$t\f$.
    unsigned char* concat = (unsigned char*)calloc(2*hashLen, sizeof(unsigned char));
    for(int i = 0; i < hashLen; i++)
    {
        concat[i] = w[i];
    }
    for(int i = 0; i < hashLen; i++)
    {
        concat[hashLen + i] = t[i];
    }

    mpz_t v;
    mpz_init(v);
    hashToRange(v, concat, 2*hashLen, publicParameters.q, publicParameters.hashFunction);

    // Let \f$u = v \cdot \mathrm{privateKey} + k \cdot Q_{id}\f$ be a point on the elliptic-curve,
    // part of the signature.
    AffinePoint u, kMulPointQId, vMulPrivateKey;
    status = affine_wNAFMultiply(&vMulPrivateKey, v, privateKey, publicParameters.ellipticCurve);
    if(status)
    {
        mpz_clears(k, v, NULL);
        affine_destroy(pointQId);
        complex_destroyMany(2, theta, r);
        free(z);
        free(w);
        free(t);
        free(concat);
        return status;
    }
    status = affine_wNAFMultiply(&kMulPointQId, k, pointQId, publicParameters.ellipticCurve);
    if(status)
    {
        mpz_clears(k, v, NULL);
        affine_destroy(pointQId);
        affine_destroy(vMulPrivateKey);
        complex_destroyMany(2, theta, r);
        free(z);
        free(w);
        free(t);
        free(concat);
        return status;
    }
    status = affine_add(&u, vMulPrivateKey, kMulPointQId, publicParameters.ellipticCurve);
    if(status)
    {
        mpz_clears(k, v, NULL);
        affine_destroy(pointQId);
        affine_destroy(vMulPrivateKey);
        affine_destroy(kMulPointQId);
        complex_destroyMany(2, theta, r);
        free(z);
        free(w);
        free(t);
        free(concat);
        return status;
    }

    *result = signature_init(u, v);

    mpz_clears(k, v, NULL);
    affine_destroy(pointQId);
    affine_destroy(vMulPrivateKey);
    affine_destroy(kMulPointQId);
    affine_destroy(u);
    complex_destroyMany(2, theta, r);
    free(z);
    free(w);
    free(t);
    free(concat);

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_ibs_hess_verify(const char *const message, const size_t messageLength, const Signature signature,
                    const char *const identity, const size_t identityLength, const PublicParameters publicParameters)
{
    // Implementation of Scheme 1. Verify in [HESS-IBS].

    if(!message)
    {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(messageLength == 0)
    {
        return CRYPTID_MESSAGE_LENGTH_ERROR;
    }

    if(!validation_isSignatureValid(signature, publicParameters.ellipticCurve.fieldOrder))
    {
        return CRYPTID_ILLEGAL_SIGNATURE_ERROR;
    }

    if(!identity)
    {
        return CRYPTID_IDENTITY_NULL_ERROR;
    }

    if(identityLength == 0)
    {
        return CRYPTID_IDENTITY_LENGTH_ERROR;
    }

    if(!validation_isPublicParametersValid(publicParameters))
    {
        return CRYPTID_ILLEGAL_PUBLIC_PARAMETERS_ERROR;
    }

    CryptidStatus status;

    // Let {@code hashlen} be the length of the output of the hash function
    // {@code hashfcn} measured in octets.
    int hashLen = hashFunction_getHashSize(publicParameters.hashFunction);

    // Let \f$theta1 = \mathrm{Pairing}(E, p ,q, u, P_{pub})\f$ by applying the modified
    // Tate pairing.
    Complex theta1;
    status = tate_performPairing(&theta1, 2, publicParameters.ellipticCurve, publicParameters.q, signature.u, publicParameters.pointP);
    if(status)
    {
        return status;
    }

    // \f$Q_{id} = \mathrm{HashToPoint}(E, p, q, id, \mathrm{hashfcn})\f$
    // which results in a point of order \f$q\f$ in \f$E(F_p)\f$.
    AffinePoint pointQId;
    status = hashToPoint(&pointQId, publicParameters.ellipticCurve, publicParameters.ellipticCurve.fieldOrder, publicParameters.q, identity, identityLength, publicParameters.hashFunction);
    if(status)
    {
        complex_destroy(theta1);
        return status;
    }

    // Let (@code negativePointPpublic) be \f$-P_{pub}\f$.
    AffinePoint negativePointPpublic;
    mpz_t yNegate, yNegateModP;
    mpz_inits(yNegate, yNegateModP, NULL);
    mpz_neg(yNegate, publicParameters.pointPpublic.y);
    mpz_mod(yNegateModP, yNegate, publicParameters.ellipticCurve.fieldOrder);

    negativePointPpublic = affine_init(publicParameters.pointPpublic.x, yNegateModP);

    // Let \f$theta2 = \mathrm{Pairing}(E, p , q, Q_{id}, \f$-P_{pub}\f$)\f$ by applying the modified
    // Tate pairing.
    Complex theta2;
    status = tate_performPairing(&theta2, 2, publicParameters.ellipticCurve, publicParameters.q, pointQId, negativePointPpublic);
    if(status)
    {
        complex_destroy(theta1);
        affine_destroy(pointQId);
        affine_destroy(negativePointPpublic);
        mpz_clears(yNegate, yNegateModP, NULL);
        return status;
    }

    // Let \f$\mathrm{theta2}^{\prime} = \mathrm{theta2}^v\f$, which is theta raised to the power of \f$v\f$ in \f$F_p^2\f$.
    Complex theta2Prime = complex_modPow(theta2, signature.v, publicParameters.ellipticCurve.fieldOrder);

    // Let \f$ r = \mathrm{theta1} \cdot \mathrm{theta2}^{\prime} \f$
    Complex r = complex_modMul(theta1, theta2Prime, publicParameters.ellipticCurve.fieldOrder);

    // Verify that the signature (@code v) equals with the now computed value.
    // The code is the same as in the sign method.
    int zLength;
    unsigned char* z = canonical(&zLength, publicParameters.ellipticCurve.fieldOrder, r, 1);

    unsigned char* w = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    hashFunction_hash(publicParameters.hashFunction, z, zLength, w);

    unsigned char* t = (unsigned char*)calloc(hashLen, sizeof(unsigned char));
    hashFunction_hash(publicParameters.hashFunction, (unsigned char*) message, messageLength, t);

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

    // If the values were the same, the verification returns succes.
    if(mpz_cmp(signature.v, v) == 0)
    {
        complex_destroyMany(4, theta1, theta2, theta2Prime, r);
        affine_destroy(pointQId);
        affine_destroy(negativePointPpublic);
        mpz_clears(yNegate, yNegateModP, v, NULL);
        free(z);
        free(w);
        free(t);
        free(concat);
        return CRYPTID_SUCCESS;
    }

    // Otherwise, failure.
    complex_destroyMany(4, theta1, theta2, theta2Prime, r);
    affine_destroy(pointQId);
    affine_destroy(negativePointPpublic);
    mpz_clears(yNegate, yNegateModP, v, NULL);
    free(z);
    free(w);
    free(t);
    free(concat);

    return CRYPTID_VERIFICATION_FAILED_ERROR;
}
