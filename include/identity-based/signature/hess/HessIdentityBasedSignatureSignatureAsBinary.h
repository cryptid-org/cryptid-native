#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_AS_BINARY_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_SIGNATURE_AS_BINARY_H

#include <stddef.h>

#include "elliptic/AffinePointAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"

/**
 * ## Description
 *
 * Struct holding the signature data in binary form for easier serialization.
 */
typedef struct HessIdentityBasedSignatureSignatureAsBinary {
  /**
   * ## Description
   *
   * A binary representation of a point in \f$E(F_p)\f$ representing a part of
   * the signature.
   */
  AffinePointAsBinary u;

  /**
   * ## Description
   *
   * A binary string representing a part of the signature.
   */
  void *v;

  /**
   * ## Description
   *
   * The length of
   * [v](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary.v).
   */
  size_t vLength;
} HessIdentityBasedSignatureSignatureAsBinary;

/**
 * ## Description
 *
 * Initializes a new
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * with the specified values.
 *
 * ## Parameters
 *
 *   * signatureAsBinaryOutput
 *     * The
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * to be initialized.
 *   * u
 *     * A binary representation of a point in \f$E(F_p)\f$ representing a part
 * of the signature.
 *   * v
 *     * A binary string representing a part of the signature.
 *   * vLength
 *     * The length of
 * [v](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary.v).
 */
void hessIdentityBasedSignatureSignatureAsBinary_init(
    HessIdentityBasedSignatureSignatureAsBinary *signatureAsBinaryOutput,
    const AffinePointAsBinary u, const void *const v, const size_t vLength);

/**
 * ## Description
 *
 * Frees a
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 *
 * ## Parameters
 *
 *   * signatureAsBinary
 *     * The
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * to be destroyed.
 */
void hessIdentityBasedSignatureSignatureAsBinary_destroy(
    HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary);

/**
 * ## Description
 *
 * Converts a
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * to
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 *
 * ## Parameters
 *
 *   * signatureOutput
 *     * The
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * to be initialized based on a
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 *   * signatureAsBinary
 *     * The
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary),
 * which needs to be converted to
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 */
void hessIdentityBasedSignatureSignatureAsBinary_toHessIdentityBasedSignatureSignature(
    HessIdentityBasedSignatureSignature *signatureOutput,
    const HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary);

/**
 * ## Description
 *
 * Converts a
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * to
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 *
 * ## Parameters
 *
 *   * signatureAsBinaryOutput
 *     * The
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * to be initialized based on a
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 *   * signature
 *     * The
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary),
 * which needs to be converted to
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary).
 */
void hessIdentityBasedSignatureSignatureAsBinary_fromHessIdentityBasedSignatureSignature(
    HessIdentityBasedSignatureSignatureAsBinary *signatureAsBinaryOutput,
    const HessIdentityBasedSignatureSignature signature);

#endif
