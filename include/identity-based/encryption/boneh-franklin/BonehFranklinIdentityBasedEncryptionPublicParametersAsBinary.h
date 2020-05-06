#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_BINARY_H

#include "elliptic/AffinePointAsBinary.h"
#include "elliptic/EllipticCurveAsBinary.h"
#include "util/HashFunction.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h"

/**
 * ## Description
 *
 * Struct storing the BF-IBE public parameters in binary form for easier serialization.
 */
typedef struct BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
{
    /**
     * ## Description
     *
     * The Type-1 elliptic curve we're operating over represented by binary values.
     */
    EllipticCurveAsBinary ellipticCurve;

    /**
     * ## Description
     *
     * Subgroup order represented as a binary.
     */
    void *q;

    /**
     * ## Description
     *
     * The length of [q](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#q).
     */
    size_t qLength;

    /**
     * The binary representation of a generator point of group order [q](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#q) on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve).
     */
    AffinePointAsBinary pointP;

    /**
     * The binary representation of a point on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve) which is [pointP](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#pointP) multiplied with the master secret.
     */
    AffinePointAsBinary pointPpublic;

    /**
     * The hash function, used by the system.
     */
    HashFunction hashFunction;
} BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary;

/**
 * ## Description
 *
 * Initializes a new [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary) with the specified values.
 *
 * ## Parameters
 *
 *   * publicParametersAsBinaryOutput
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary) to be initialized.
 *   * ellipticCurve
 *     * The [EllipticCurve](elliptic/EllipticCurve.h#EllipticCurve), the system operates on, but in binary representation.
 *   * q
 *     * The binary representation pf the field order of the subgroup, the system operates on.
 *   * qLength
 *     * The length of [q](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#q).
 *   * pointP
 *     * The binary representation of a generator point of group order [q](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#q) on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve).
 *   * pointPpublic
 *     * The binary representation of a point on [ellipticCurve](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#ellipticCurve) which is [pointP](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#pointP) multiplied with the master secret.
 *   * hashFunction
 *     * The hash function, used by the system.
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_init(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary *publicParametersAsBinaryOutput, const EllipticCurveAsBinary ellipticCurve, const void *const q, const size_t qLength, const AffinePointAsBinary pointP, const AffinePointAsBinary pointPpublic, const HashFunction hashFunction);

/**
 * ## Description
 *
 * Frees a [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersAsBinary
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary) to be destroyed.
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

/**
 * ## Description
 *
 * Converts a [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary) to [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 *
 * ## Parameters
 *
 *   * publicParametersOutput
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters) to be initialized based on a [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 *   * publicParametersAsBinary
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary), which needs to be converted to [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_toBonehFranklinIdentityBasedEncryptionPublicParameters(BonehFranklinIdentityBasedEncryptionPublicParameters *publicParametersOutput, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

/**
 * ## Description
 *
 * Converts a [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters) to [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersAsBinaryOutput
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary) to be initialized based on a [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 *   * publicParameters
 *     * The [BonehFranklinIdentityBasedEncryptionPublicParameters](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters), which needs to be converted to [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_fromBonehFranklinIdentityBasedEncryptionPublicParameters(BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary *publicParametersAsBinaryOutput, const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

#endif