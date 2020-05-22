#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_PUBLICPARAMETERS_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * Struct storing the Hess-IBS Public Parameters.
 */
typedef struct HessIdentityBasedSignaturePublicParameters {
  /**
   * ## Description
   *
   * The Type-1 elliptic curve we're operating over.
   */
  EllipticCurve ellipticCurve;

  /**
   * ## Description
   *
   * Subgroup order.
   */
  mpz_t q;

  /**
   * ## Description
   *
   * A generator point of group order
   * [q](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.q)
   * on
   * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve).
   */
  AffinePoint pointP;

  /**
   * ## Description
   *
   * A point on
   * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve)
   * which is
   * [pointP](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.pointP)
   * multiplied with the master secret.
   */
  AffinePoint pointPpublic;

  /**
   * ## Description
   *
   * The used hash function.
   */
  HashFunction hashFunction;
} HessIdentityBasedSignaturePublicParameters;

/**
 * ## Description
 *
 * Initializes a new
 * [HessIdentityBasedSignaturePublicParameters](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters)
 * with the specified values.
 *
 * ## Parameters
 *
 *   * publicParametersOutput
 *     * The
 * [HessIdentityBasedSignaturePublicParameters](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters)
 * to be initialized.
 *   * ellipticCurve
 *     * The [EllipticCurve](codebase://elliptic/EllipticCurve.h#EllipticCurve),
 * the system operates on.
 *   * q
 *     * The field order of the subgroup, the system operates on.
 *   * pointP
 *     * A generator point of group order
 * [q](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.q)
 * on
 * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve).
 *   * pointPpublic
 *     * A point on
 * [ellipticCurve](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.ellipticCurve)
 * which is
 * [pointP](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters.pointP)
 * multiplied with the master secret.
 *   * hashFunction
 *     * The hash function, the system operates with.
 */
void hessIdentityBasedSignaturePublicParameters_init(
    HessIdentityBasedSignaturePublicParameters *publicParametersOutput,
    const EllipticCurve ellipticCurve, const mpz_t q, const AffinePoint pointP,
    const AffinePoint pointPpublic, const HashFunction hashFunction);

/**
 * ## Description
 *
 * Frees a
 * [HessIdentityBasedSignaturePublicParameters](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters).
 *
 * ## Parameters
 *
 *   * publicParameters
 *     * The
 * [HessIdentityBasedSignaturePublicParameters](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters)
 * to be destroyed.
 */
void hessIdentityBasedSignaturePublicParameters_destroy(
    HessIdentityBasedSignaturePublicParameters publicParameters);

/**
 * ## Description
 *
 * Validates that the specified public parameters are correct.
 *
 * ## Parameters
 *
 *   * publicParameters
 *     * The
 * [HessIdentityBasedSignaturePublicParameters](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h#HessIdentityBasedSignaturePublicParameters)
 * to check.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the parameters are valid.
 */
CryptidValidationResult hessIdentityBasedSignaturePublicParameters_isValid(
    const HessIdentityBasedSignaturePublicParameters publicParameters);

#endif
