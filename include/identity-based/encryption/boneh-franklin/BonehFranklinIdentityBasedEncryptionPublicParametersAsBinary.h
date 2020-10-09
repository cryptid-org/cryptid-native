#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_PUBLICPARAMETERS_AS_BINARY_H

#include <stddef.h>

#include "elliptic/AffinePointAsBinary.h"
#include "elliptic/EllipticCurveAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h"
#include "util/HashFunction.h"

/**
 * ## Description
 *
 * Struct storing the BF-IBE public parameters in binary form for easier
 * serialization.
 */
typedef struct BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary {
  /**
   * ## Description
   *
   * The Type-1 elliptic curve we're operating over represented by binary
   * values.
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
   * The length of
   * [q](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.q).
   */
  size_t qLength;

  /**
   * The binary representation of a generator point of group order
   * [q](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.q)
   * on
   * [ellipticCurve](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.ellipticCurve).
   */
  AffinePointAsBinary pointP;

  /**
   * The binary representation of a point on
   * [ellipticCurve](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.ellipticCurve)
   * which is
   * [pointP](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.pointP)
   * multiplied with the master secret.
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
 * Initializes a new
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary)
 * with the specified values.
 *
 * ## Parameters
 *
 *   * publicParametersAsBinaryOutput
 *     * The
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary)
 * to be initialized.
 *   * ellipticCurve
 *     * The [EllipticCurve](codebase://elliptic/EllipticCurve.h#EllipticCurve),
 * the system operates on, but in binary representation.
 *   * q
 *     * The binary representation pf the field order of the subgroup, the
 * system operates on.
 *   * qLength
 *     * The length of
 * [q](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.q).
 *   * pointP
 *     * The binary representation of a generator point of group order
 * [q](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.q)
 * on
 * [ellipticCurve](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.ellipticCurve).
 *   * pointPpublic
 *     * The binary representation of a point on
 * [ellipticCurve](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.ellipticCurve)
 * which is
 * [pointP](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters.pointP)
 * multiplied with the master secret.
 *   * hashFunction
 *     * The hash function, used by the system.
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_init(
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersAsBinaryOutput,
    const EllipticCurveAsBinary ellipticCurve, const void *const q,
    const size_t qLength, const AffinePointAsBinary pointP,
    const AffinePointAsBinary pointPpublic, const HashFunction hashFunction);

/**
 * ## Description
 *
 * Frees a
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersAsBinary
 *     * The
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary)
 * to be destroyed.
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary);

/**
 * ## Description
 *
 * Converts a
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary)
 * to
 * [BonehFranklinIdentityBasedEncryptionPublicParameters](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 *
 * ## Parameters
 *
 *   * publicParametersOutput
 *     * The
 * [BonehFranklinIdentityBasedEncryptionPublicParameters](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters)
 * to be initialized based on a
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 *   * publicParametersAsBinary
 *     * The
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary),
 * which needs to be converted to
 * [BonehFranklinIdentityBasedEncryptionPublicParameters](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_toBonehFranklinIdentityBasedEncryptionPublicParameters(
    BonehFranklinIdentityBasedEncryptionPublicParameters
        *publicParametersOutput,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary);

/**
 * ## Description
 *
 * Converts a
 * [BonehFranklinIdentityBasedEncryptionPublicParameters](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters)
 * to
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersAsBinaryOutput
 *     * The
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary)
 * to be initialized based on a
 * [BonehFranklinIdentityBasedEncryptionPublicParameters](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters).
 *   * publicParameters
 *     * The
 * [BonehFranklinIdentityBasedEncryptionPublicParameters](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h#BonehFranklinIdentityBasedEncryptionPublicParameters),
 * which needs to be converted to
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary).
 */
void bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_fromBonehFranklinIdentityBasedEncryptionPublicParameters(
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersAsBinaryOutput,
    const BonehFranklinIdentityBasedEncryptionPublicParameters
        publicParameters);

#endif