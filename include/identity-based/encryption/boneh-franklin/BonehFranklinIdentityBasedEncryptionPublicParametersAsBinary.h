#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_BINARY_H

#include "elliptic/AffinePointAsBinary.h"
#include "elliptic/EllipticCurveAsBinary.h"
#include "util/HashFunction.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h"

/**
 * Struct storing the BF-IBE Public Parameters. Corresponds to {@code BFPublicParameters} in [RFC-5091].
 */
typedef struct BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
{
    /**
     * The Type-1 elliptic curve we're operating over represented by binary values. Note, that the {@code p} field of {@code BFPublicParameters}
     * corresponds to {@code ellipticCurve.fieldOrder}.
     */
    EllipticCurveAsBinary ellipticCurve;

    /**
     * Subgroup order represented as a binary.
     */
    void *q;

    /**
     * The length of the Subgroup order's binary representation.
     */
    size_t qLength;

    /**
     * The string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsBinary pointP;

    /**
     * The string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsBinary pointPpublic;

    /**
     * The used hash function.
     */
    HashFunction hashFunction;
} BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary;

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_init(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary *publicParametersAsBinaryOutput, const EllipticCurveAsBinary ellipticCurve, const void *const q, const size_t qLength, const AffinePointAsBinary pointP, const AffinePointAsBinary pointPpublic, const HashFunction hashFunction);

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_toBonehFranklinIdentityBasedEncryptionPublicParameters(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_fromBonehFranklinIdentityBasedEncryptionPublicParameters(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary *publicParametersAsBinaryOutput, const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

#endif