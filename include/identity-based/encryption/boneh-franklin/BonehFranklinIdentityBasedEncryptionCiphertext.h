#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_H

#include <stddef.h>

#include "elliptic/AffinePoint.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * Struct holding the BF-IBE ciphertext. Refer to
 * [BFCiphertextBlock](https://tools.ietf.org/html/rfc5091#page-56) in
 * [RFC-5091](https://tools.ietf.org/html/rfc5091).
 */
typedef struct BonehFranklinIdentityBasedEncryptionCiphertext {
  /**
   * ## Description
   *
   * A point in \f$E(F_p)\f$ representing a part of the cipher.
   */
  AffinePoint cipherU;

  /**
   * ## Description
   *
   * A binary string representing a part of the cipher.
   */
  unsigned char *cipherV;

  /**
   * ## Description
   *
   * The length of
   * [cipherV](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext.cipherV).
   */
  size_t cipherVLength;

  /**
   * ## Description
   *
   * A binary string representing a part of the cipher.
   */
  unsigned char *cipherW;

  /**
   * ## Description
   *
   * The length of
   * [cipherW](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext.cipherW).
   */
  size_t cipherWLength;
} BonehFranklinIdentityBasedEncryptionCiphertext;

/**
 * ## Description
 *
 * Initializes a new
 * [BonehFranklinIdentityBasedEncryptionCiphertext](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext)
 * with the specified values. Note, that
 * [cipherV](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext.cipherV)
 * and
 * [cipherW](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext.cipherW)
 * will be copied.
 *
 * ## Parameters
 *
 *   * ciphertextOutput
 *     * The
 * [BonehFranklinIdentityBasedEncryptionCiphertext](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext)
 * to be initialized.
 *   * cipherU
 *     * A point in \f$E(F_p)\f$ representing a part of the signature.
 *   * cipherV
 *     * A binary string representing a part of the cipher.
 *   * cipherVLength
 *     * The length of
 * [cipherV](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext.cipherV).
 *   * cipherW
 *     * A binary string representing a part of the cipher.
 *   * cipherWLength
 *     * The length of
 * [cipherW](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext.cipherW).
 */
void bonehFranklinIdentityBasedEncryptionCiphertext_init(
    BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput,
    const AffinePoint cipherU, const unsigned char *const cipherV,
    const int cipherVLength, const unsigned char *const cipherW,
    const int cipherWLength);

/**
 * ## Description
 *
 * Frees a
 * [BonehFranklinIdentityBasedEncryptionCiphertext](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext).
 *
 * ## Parameters
 *
 *   * bonehFranklinIdentityBasedEncryptionCiphertext
 *     * The
 * [BonehFranklinIdentityBasedEncryptionCiphertext](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext)
 * to be destroyed.
 */
void bonehFranklinIdentityBasedEncryptionCiphertext_destroy(
    BonehFranklinIdentityBasedEncryptionCiphertext ciphertext);

/**
 * ## Description
 *
 * Validates that the specified ciphertext is correct.
 *
 * ## Parameters
 *
 *   * ciphertext
 *     * The
 * [BonehFranklinIdentityBasedEncryptionCiphertext](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext)
 * to check.
 *   * ellipticCurve
 *     * The elliptic curve, we are operating over.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the ciphertext is valid.
 */
CryptidValidationResult bonehFranklinIdentityBasedEncryptionCiphertext_isValid(
    const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext,
    const EllipticCurve ellipticCurve);

#endif
