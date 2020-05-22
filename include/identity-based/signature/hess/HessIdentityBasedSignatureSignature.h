#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * Struct holding the Hess-IBS signature.
 */
typedef struct HessIdentityBasedSignatureSignature {
  /**
   * ## Description
   *
   * A point in \f$E(F_p)\f$ representing a part of the signature.
   */
  AffinePoint u;

  /**
   * ## Description
   *
   * An integer representing a part of the signature.
   */
  mpz_t v;
} HessIdentityBasedSignatureSignature;

/**
 * ## Description
 *
 * Initializes a new
 * [HessIdentityBasedSignatureSignature](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignature.h#HessIdentityBasedSignatureSignature)
 * with the specified values.
 *
 * ## Parameters
 *
 *   * signatureOutput
 *     * The
 * [HessIdentityBasedSignatureSignature](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignature.h#HessIdentityBasedSignatureSignature)
 * to be initialized.
 *   * u
 *     * A point in \f$E(F_p)\f$ representing a part of the signature.
 *   * v
 *     * An integer representing a part of the signature.
 */
void hessIdentityBasedSignatureSignature_init(
    HessIdentityBasedSignatureSignature *signatureOutput, const AffinePoint u,
    const mpz_t v);

/**
 * ## Description
 *
 * Frees a
 * [HessIdentityBasedSignatureSignature](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignature.h#HessIdentityBasedSignatureSignature).
 *
 * ## Parameters
 *
 *   * signature
 *     * The
 * [HessIdentityBasedSignatureSignature](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignature.h#HessIdentityBasedSignatureSignature)
 * to be destroyed.
 */
void hessIdentityBasedSignatureSignature_destroy(
    HessIdentityBasedSignatureSignature signature);

/**
 * ## Description
 *
 * Validates that the specified signature is correct.
 *
 * ## Parameters
 *
 *   * signature
 *     * The
 * [HessIdentityBasedSignatureSignature](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignature.h#HessIdentityBasedSignatureSignature)
 * to check.
 *   * ellipticCurve
 *     * The elliptic curve, we are operating over.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the signature is valid.
 */
CryptidValidationResult hessIdentityBasedSignatureSignature_isValid(
    const HessIdentityBasedSignatureSignature signature,
    const EllipticCurve ellipticCurve);

#endif
