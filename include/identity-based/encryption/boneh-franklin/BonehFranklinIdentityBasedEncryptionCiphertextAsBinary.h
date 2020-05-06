#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_AS_BINARY_H

#include <stddef.h>

#include "elliptic/AffinePointAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h"

/**
 * ## Description
 *
 * Struct holding the ciphertext data in binary form for easier serialization. Refer to [BFCiphertextBlock](https://tools.ietf.org/html/rfc5091#page-56) in [RFC-5091](https://tools.ietf.org/html/rfc5091).
 */
typedef struct BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
{
    /**
     * ## Description
     *
     * A binary representation of a point in \f$E(F_p)\f$ representing a part of the cipher.
     */
    AffinePointAsBinary cipherU;

    /**
     * ## Description
     *
     * A binary string representing a part of the cipher.
     */
    void *cipherV;

    size_t cipherVLength;

    /**
     * ## Description
     *
     * A binary string representing a part of the cipher.
     */
    void *cipherW;

    size_t cipherWLength;
} BonehFranklinIdentityBasedEncryptionCiphertextAsBinary;

/**
 * ## Description
 *
 * Initializes a new [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary) with the specified values. Note, that [cipherV](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#cipherV) and [cipherW](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#cipherW) will be copied.
 * 
 * ## Parameters
 *
 *   * ciphertextAsBinaryOutput
 *     * The [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary) to be initialized.
 *   * cipherU
 *     * An [AffinePointAsBinary](elliptic/AffinePointAsBinary.h#AffinePointAsBinary) representing a part of the cipher.
 *   * cipherV
 *     * A binary string representing a part of the cipher.
 *   * cipherVLength
 *     * The length of [cipherV](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#cipherV).
 *   * cipherW
 *     * A binary string representing a part of the cipher.
 *   * cipherWLength
 *     * The length of [cipherW](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#cipherW).
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_init(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertextAsBinaryOutput, const AffinePointAsBinary cipherU, const void *const cipherV, const size_t cipherVLength, const void *const cipherW, const size_t cipherWLength);

/**
 * ## Description
 *
 * Frees a [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary).
 *
 * ## Parameters
 *
 *   * bonehFranklinIdentityBasedEncryptionCiphertextAsBinary
 *     * The [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary) to be destroyed.
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary bonehFranklinIdentityBasedEncryptionCiphertextAsBinary);

/**
 * ## Description
 *
 * Converts an [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary) to [BonehFranklinIdentityBasedEncryptionCiphertext](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext).
 *
 * ## Parameters
 *
 *   * ciphertextOutput
 *     * The [BonehFranklinIdentityBasedEncryptionCiphertext](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext) to be initialized based on a [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary).
 *   * ciphertextAsBinary
 *     * The [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary), which needs to be converted to [BonehFranklinIdentityBasedEncryptionCiphertext](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext).
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_toBonehFranklinIdentityBasedEncryptionCiphertext(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertextAsBinary);

/**
 * ## Description
 *
 * Converts an [BonehFranklinIdentityBasedEncryptionCiphertext](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext) to [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary).
 *
 * ## Parameters
 *
 *   * ciphertextAsBinaryOutput
 *     * The [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary) to be initialized based on a [BonehFranklinIdentityBasedEncryptionCiphertext](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext).
 *   * ciphertext
 *     * The [BonehFranklinIdentityBasedEncryptionCiphertext](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h#BonehFranklinIdentityBasedEncryptionCiphertext), which needs to be converted to [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary).
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_fromBonehFranklinIdentityBasedEncryptionCiphertext(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertextAsBinaryOutput, const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext);

#endif
