/**
 * # CryptID Identity-based Encryption
 *
 * ## Excerpt
 *
 * Routines implementing the Boneh-Franklin Identity-based Encryption scheme.
 */
#ifndef __CRYPTID_CRYPTID_H
#define __CRYPTID_CRYPTID_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/CipherTextTuple.h"
#include "identity-based/PublicParameters.h"
#include "identity-based/SecurityLevel.h"
#include "util/Status.h"


/**
 * ---
 * showing-off: How YAML attributes work.
 * ---
 *
 * ## Description
 *
 * Establishes a master secret and public parameters for a given security level. The master secret (as its name suggests)
 * should be kept secret, while the public parameters can be distributed among the clients.
 *
 * ## Parameters
 *
 *   * securityLevel
 *     * The desired security level.
 *   * publicParameters
 *     * Pointer in which the public parameters will be stored. If the return value is CRYPTID_SUCCESS
 *       then it will point to correctly filled PublicParameters instance. Note, that the
 *       q field must be mpz_init'd and thus mpz_clear'd be the caller.
 *   * masterSecret
 *     * Out parameter which will hold the master secret. Must be mpz_init'd and mpz_clear'd by the caller.
 *
 * ## Return Value
 * 
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_setup(const SecurityLevel securityLevel, PublicParameters* publicParameters, mpz_t masterSecret);

/**
 * ## Description
 *
 * Extracts the private key corresponding to a given identity string.
 *
 * ## Parameters
 * 
 *   * result
 *     * Out parameter holding the private key (which is an AffinePoint). On CRYPTID_SUCCESS, it must be destroyed by 
 *       the caller. Initialization is done by this function.
 *   * identity
 *     * The identity string we're extracting the private key for.
 *   * identityLength
 *     * The length of the identity string.
 *   * publicParameters
 *     * The IBE public parameters.
 *   * masterSecret
 *     * the master secret corresponding to the public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_extract(AffinePoint* result, const char *const identity, const size_t identityLength, 
                       const PublicParameters publicParameters, const mpz_t masterSecret);

/**
 * ## Description
 *
 * Encrypts a message with the given identity string.
 *
 * ## Parameters
 *
 *   * result 
 *     * Out parameter storing the ciphertext. If the return value is CRYPTID_SUCCESS
 *       then it will point to an CipherTextTuple instance, that must be destroyed by the caller.
 *       Initialization is done by this function.
 *   * message
 *     * The string to encrypt.
 *   * messageLength
 *     * The length of the message.
 *   * identity
 *     * The identity string to encrypt with.
 *   * identityLength
 *     * The length of the identity.
 *   * publicParameters
 *     * The IBE public parameters.
 *
 * ## Return Value
 * 
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_encrypt(CipherTextTuple *result, const char *const message, const size_t messageLength,
                       const char *const identity, const size_t identityLength, const PublicParameters publicParameters);

/**
 * ## Description
 *
 * Decrypts a previously encrypted message using the specified private key.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter holding the message in plaintext. If the return value is CRYPTID_SUCCESS
 *       then it will point to a zero-terminated string, that must be destroyed by the caller.
 *   * privateKey
 *     * The private key to decrypt with.
 *   * ciphertext
 *     * The ciphertext to decrypt.
 *   * publicParameters
 *     * The IBE public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_decrypt(char **result, const AffinePoint privateKey, const CipherTextTuple ciphertext, 
                       const PublicParameters publicParameters);

#endif
