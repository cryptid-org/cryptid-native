#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_AS_BINARY_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_AS_BINARY_H

#include "elliptic/AffinePointAsBinary.h"
#include "elliptic/EllipticCurveAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h"
#include "util/HashFunction.h"

/**
 * ## Description
 *
 * Struct storing the Hess-IBS public parameters in binary form for easier
 * serialization.
 */
typedef struct HessIdentityBasedSignaturePublicParametersAsBinary {
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
   * [q](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary.q).
   */
  size_t qLength;

  /**
   * The binary representation of a generator point of group order
   * [q](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.q)
   * on
   * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve).
   */
  AffinePointAsBinary pointP;

  /**
   * The binary representation of a point on
   * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve)
   * which is
   * [pointP](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.pointP)
   * multiplied with the master secret.
   */
  AffinePointAsBinary pointPpublic;

  /**
   * The hash function, used by the system.
   */
  HashFunction hashFunction;
} HessIdentityBasedSignaturePublicParametersAsBinary;

/**
 * ## Description
 *
 * Initializes a new
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * with the specified values.
 *
 * ## Parameters
 *
 *   * publicParametersAsBinaryOutput
 *     * The
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * to be initialized.
 *   * ellipticCurve
 *     * The [EllipticCurve](codebase://elliptic/EllipticCurve.h#EllipticCurve),
 * the system operates on, but in binary representation.
 *   * q
 *     * The binary representation pf the field order of the subgroup, the
 * system operates on.
 *   * qLength
 *     * The length of
 * [q](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary.q).
 *   * pointP
 *     * The binary representation of a generator point of group order
 * [q](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.q)
 * on
 * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve).
 *   * pointPpublic
 *     * The binary representation of a point on
 * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve)
 * which is
 * [pointP](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.pointP)
 * multiplied with the master secret.
 *   * hashFunction
 *     * The hash function, used by the system.
 */
void hessIdentityBasedSignaturePublicParametersAsBinary_init(
    HessIdentityBasedSignaturePublicParametersAsBinary
        *publicParametersAsBinaryOutput,
    const EllipticCurveAsBinary ellipticCurve, const void *const q,
    const size_t qLength, const AffinePointAsBinary pointP,
    const AffinePointAsBinary pointPpublic, const HashFunction hashFunction);

/**
 * ## Description
 *
 * Frees a
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersAsBinary
 *     * The
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * to be destroyed.
 */
void hessIdentityBasedSignaturePublicParametersAsBinary_destroy(
    HessIdentityBasedSignaturePublicParametersAsBinary
        publicParametersAsBinary);

/**
 * ## Description
 *
 * Converts a
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * to
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersOutput
 *     * The
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * to be initialized based on a
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 *   * publicParametersAsBinary
 *     * The
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary),
 * which needs to be converted to
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 */
void hessIdentityBasedSignaturePublicParametersAsBinary_toHessIdentityBasedSignaturePublicParameters(
    HessIdentityBasedSignaturePublicParameters *publicParametersOutput,
    const HessIdentityBasedSignaturePublicParametersAsBinary
        publicParametersAsBinary);

/**
 * ## Description
 *
 * Converts a
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * to
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 *
 * ## Parameters
 *
 *   * publicParametersAsBinaryOutput
 *     * The
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * to be initialized based on a
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 *   * publicParameters
 *     * The
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary),
 * which needs to be converted to
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary).
 */
void hessIdentityBasedSignaturePublicParametersAsBinary_fromHessIdentityBasedSignaturePublicParameters(
    HessIdentityBasedSignaturePublicParametersAsBinary
        *publicParametersAsBinaryOutput,
    const HessIdentityBasedSignaturePublicParameters publicParameters);

#endif
