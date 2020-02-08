#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_STRING_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_STRING_H

#include "elliptic/AffinePointAsString.h"
#include "elliptic/EllipticCurveAsString.h"
#include "util/HashFunction.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h"

/**
 * Struct storing the BF-IBE Public Parameters. Corresponds to {@code BFPublicParameters} in [RFC-5091].
 */
typedef struct BonehFranklinIdentityBasedEncryptionPublicParametersAsString
{
    /**
     * The Type-1 elliptic curve we're operating over represented by string values. Note, that the {@code p} field of {@code BFPublicParameters}
     * corresponds to {@code ellipticCurve.fieldOrder}.
     */
    EllipticCurveAsString ellipticCurve;

    /**
     * Subgroup order represented as a string.
     */
    char *q;

    /**
     * The base of the Subgroup order's string representation.
     */
    int qBase;

    /**
     * The string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsString pointP;

    /**
     * The string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsString pointPpublic;

    /**
     * The used hash function.
     */
    HashFunction hashFunction;
} BonehFranklinIdentityBasedEncryptionPublicParametersAsString;

void bonehFranklinIdentityBasedEncryptionPublicParametersAsString_init(BonehFranklinIdentityBasedEncryptionPublicParametersAsString *publicParametersAsStringOutput, const EllipticCurveAsString ellipticCurve, const char *const q, const int qBase, const AffinePointAsString pointP, const AffinePointAsString pointPpublic, const HashFunction hashFunction);

void bonehFranklinIdentityBasedEncryptionPublicParametersAsString_destroy(BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString);

void bonehFranklinIdentityBasedEncryptionPublicParametersAsString_toBonehFranklinIdentityBasedEncryptionPublicParameters(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString);

void bonehFranklinIdentityBasedEncryptionPublicParameters_toBonehFranklinIdentityBasedEncryptionPublicParametersAsString(BonehFranklinIdentityBasedEncryptionPublicParametersAsString *publicParametersAsStringOutput, const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters, const int base);

#endif