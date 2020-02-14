#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_CIPHERTEXT_H

#include <stddef.h>

#include "elliptic/AffinePoint.h"
#include "util/Validation.h"

// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


/**
 * Struct holding the ciphertext data. Refer to {@code BFCiphertextBlock} in [RFC-5091].
 */
typedef struct BonehFranklinIdentityBasedEncryptionCiphertext
{
    /**
     * A point in \f$E(F_p)\f$.
     */
    AffinePoint cipherU;

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
} BonehFranklinIdentityBasedEncryptionCiphertext;


/**
 * Initializes a new BonehFranklinIdentityBasedEncryptionCiphertext with the specified values. Note, that {@code cipherV} and {@code cipherw}
 * will be copied.
 * @param ciphertextOutput the BonehFranklinIdentityBasedEncryptionCiphertext to be initialized
 * @param cipherU an AffinePoint
 * @param cipherV binary string
 * @param cipherVLength the size of {@code cipherV}
 * @param cipherW binary string
 * @param cipherWLength the size of {@code cipherW}
 */
void bonehFranklinIdentityBasedEncryptionCiphertext_init(BonehFranklinIdentityBasedEncryptionCiphertext *ciphertextOutput, const AffinePoint cipherU, const unsigned char *const cipherV, const int cipherVLength,
                                     const unsigned char *const cipherW, const int cipherWLength);

/**
 * Frees a BonehFranklinIdentityBasedEncryptionCiphertext.
 * @param bonehFranklinIdentityBasedEncryptionCiphertext the BonehFranklinIdentityBasedEncryptionCiphertext to be destroyed
 */
void bonehFranklinIdentityBasedEncryptionCiphertext_destroy(BonehFranklinIdentityBasedEncryptionCiphertext ciphertext);

/**
 * Validates that the specified ciphertext is correct.
 * @param ciphertext the ciphertext to check
 * @param order the field order of the elliptic curve field, we operating over
 * @return CRYPTID_VALIDATION_SUCCESS if the ciphertext is valid
 */
CryptidValidationResult bonehFranklinIdentityBasedEncryptionCiphertext_isValid(const BonehFranklinIdentityBasedEncryptionCiphertext ciphertext, const mpz_t order);

#endif
