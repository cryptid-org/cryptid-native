#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_AS_BINARY_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_AS_BINARY_H

#include <stddef.h>

#include "elliptic/AffinePointAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h"

/**
 * Struct holding the ciphertext data. Refer to {@code BFCiphertextBlock} in [RFC-5091].
 */
typedef struct BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
{
    /**
     * A binary representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsBinary cipherU;

    /**
     * A binary string.
     */
    unsigned char* cipherV;
    size_t cipherVLength;

    /**
     * A binary string.
     */
    unsigned char* cipherW;
    size_t cipherWLength;
} BonehFranklinIdentityBasedEncryptionCiphertextAsBinary;

/**
 * Initializes a new BonehFranklinIdentityBasedEncryptionCiphertextAsBinary with the specified values. Note, that {@code cipherV} and {@code cipherw}
 * will be copied.
 * @param ciphertextAsBinaryOutput the BonehFranklinIdentityBasedEncryptionCiphertextAsBinary to be initialized
 * @param cipherU an AffinePointAsBinary
 * @param cipherV binary string
 * @param cipherVLength the size of {@code cipherV}
 * @param cipherW binary string
 * @param cipherWLength the size of {@code cipherW}
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_init(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertextAsBinaryOutput, const AffinePointAsBinary cipherU, const unsigned char *const cipherV, const size_t cipherVLength, const unsigned char *const cipherW, const size_t cipherWLength);

/**
 * Frees a BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.
 * @param bonehFranklinIdentityBasedEncryptionCiphertextAsBinary the BonehFranklinIdentityBasedEncryptionCiphertextAsBinary to be destroyed
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary bonehFranklinIdentityBasedEncryptionCiphertextAsBinary);

/**
 * Converts an BonehFranklinIdentityBasedEncryptionCiphertextAsBinary to BonehFranklinIdentityBasedEncryptionCiphertext.
 * @param ciphertextOutput the BonehFranklinIdentityBasedEncryptionCiphertext to be initialized based on an BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
 * @param ciphertextAsBinary the BonehFranklinIdentityBasedEncryptionCiphertextAsBinary, which needs to be converted to BonehFranklinIdentityBasedEncryptionCiphertext
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_toBonehFranklinIdentityBasedEncryptionCiphertext(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertextAsBinary);

/**
 * Converts an BonehFranklinIdentityBasedEncryptionCiphertext to BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.
 * @param ciphertextAsBinaryOutput the BonehFranklinIdentityBasedEncryptionCiphertextAsBinary to be initialized based on an BonehFranklinIdentityBasedEncryptionCiphertext
 * @param ciphertext the BonehFranklinIdentityBasedEncryptionCiphertex, which needs to be converted to BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
 */
void bonehFranklinIdentityBasedEncryptionCiphertext_toBonehFranklinIdentityBasedEncryptionCiphertextAsBinary(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *ciphertextAsBinaryOutput, const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext);

#endif
