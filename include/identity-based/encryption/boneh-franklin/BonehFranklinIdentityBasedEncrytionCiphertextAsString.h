#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_AS_STRING_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_AS_STRING_H

#include <stddef.h>

#include "elliptic/AffinePointAsString.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertext.h"

/**
 * Struct holding the ciphertext data. Refer to {@code BFCiphertextBlock} in [RFC-5091].
 */
typedef struct BonehFranklinIdentityBasedEncryptionCiphertextAsString
{
    /**
     * A string representation of a point in \f$E(F_p)\f$.
     */
    AffinePointAsString cipherU;

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
} BonehFranklinIdentityBasedEncryptionCiphertextAsString;

/**
 * Initializes a new BonehFranklinIdentityBasedEncryptionCiphertextAsString with the specified values. Note, that {@code cipherV} and {@code cipherw}
 * will be copied.
 * @param ciphertextAsStringOutput the BonehFranklinIdentityBasedEncryptionCiphertextAsString to be initialized
 * @param cipherU an AffinePointAsString
 * @param cipherV binary string
 * @param cipherVLength the size of {@code cipherV}
 * @param cipherW binary string
 * @param cipherWLength the size of {@code cipherW}
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsString_init(BonehFranklinIdentityBasedEncryptionCiphertextAsString *ciphertextAsStringOutput, const AffinePointAsString cipherU, const unsigned char *const cipherV, const int cipherVLength,
                                     const unsigned char *const cipherW, const int cipherWLength);

/**
 * Frees a BonehFranklinIdentityBasedEncryptionCiphertextAsString.
 * @param bonehFranklinIdentityBasedEncryptionCiphertextAsString the BonehFranklinIdentityBasedEncryptionCiphertextAsString to be destroyed
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsString_destroy(BonehFranklinIdentityBasedEncryptionCiphertextAsString bonehFranklinIdentityBasedEncryptionCiphertextAsString);

/**
 * Converts an BonehFranklinIdentityBasedEncryptionCiphertextAsString to BonehFranklinIdentityBasedEncryptionCiphertext.
 * @param ciphertextOutput the BonehFranklinIdentityBasedEncryptionCiphertext to be initialized based on an BonehFranklinIdentityBasedEncryptionCiphertextAsString
 * @param ciphertextAsString the BonehFranklinIdentityBasedEncryptionCiphertextAsString, which needs to be converted to BonehFranklinIdentityBasedEncryptionCiphertext
 */
void bonehFranklinIdentityBasedEncryptionCiphertextAsString_toBonehFranklinIdentityBasedEncryptionCiphertext(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const BonehFranklinIdentityBasedEncryptionCiphertextAsString ciphertextAsString);

/**
 * Converts an BonehFranklinIdentityBasedEncryptionCiphertext to BonehFranklinIdentityBasedEncryptionCiphertextAsString.
 * @param ciphertextAsStringOutput the BonehFranklinIdentityBasedEncryptionCiphertextAsString to be initialized based on an BonehFranklinIdentityBasedEncryptionCiphertext
 * @param ciphertext the BonehFranklinIdentityBasedEncryptionCiphertex, which needs to be converted to BonehFranklinIdentityBasedEncryptionCiphertextAsString
 */
void bonehFranklinIdentityBasedEncryptionCiphertext_toBonehFranklinIdentityBasedEncryptionCiphertextAsString(BonehFranklinIdentityBasedEncryptionCiphertextAsString *ciphertextAsStringOutput, const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext);

#endif
