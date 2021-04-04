#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_H

#ifdef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Establishes a master secret and public parameters for a given security level.
 * The master secret (as its name suggests) should be kept secret, while the
 * public parameters can be distributed among the clients.
 *
 * ## Parameters
 *
 *   * masterSecretAsBinary
 *     * Out parameter which will hold the master secret. If the return value is
 * CRYPTID_SUCCESS, then it will point to a
 * [BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h#BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary)
 * instance, that must be destroyed by the caller. Initialization is done by
 * this function.
 *   * publicParametersAsBinary
 *     * Pointer in which the public parameters will be stored. If the return
 * value is CRYPTID_SUCCESS, then it will point to correctly filled
 * [BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h#BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary)
 * instance.
 *   * securityLevel
 *     * The desired security level.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_ibe_bonehFranklin_setup(
    BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
        *masterSecretAsBinary,
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersAsBinary,
    const SecurityLevel securityLevel);

/**
 * ## Description
 *
 * Extracts the private key corresponding to a given identity string.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter holding the private key in binary format. If the return
 * value is CRYPTID_SUCCESS, then it will point to an
 * [AffinePointAsBinary](codebase://elliptic/AffinePointAsBinary.h#AffinePointAsBinary)
 * instance, that must be destroyed by the caller. Initialization is done by
 * this function.
 *   * identity
 *     * The identity string we're extracting the private key for.
 *   * identityLength
 *     * The length of the identity string.
 *   * masterSecretAsBinary
 *     * The master secret corresponding to the public parameters.
 *   * publicParametersAsBinary
 *     * The BF-IBE public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_ibe_bonehFranklin_extract(
    AffinePointAsBinary *result, const char *const identity,
    const size_t identityLength,
    const BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
        masterSecretAsBinary,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary);

/**
 * ## Description
 *
 * Encrypts a message with the given identity string.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter storing the ciphertext. If the return value is
 * CRYPTID_SUCCESS, then it will point to a
 * [BonehFranklinIdentityBasedEncryptionCiphertextAsBinary](codebase://identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h#BonehFranklinIdentityBasedEncryptionCiphertextAsBinary)
 * instance, that must be destroyed by the caller. Initialization is done by
 * this function.
 *   * message
 *     * The string to encrypt.
 *   * messageLength
 *     * The length of the message string.
 *   * identity
 *     * The identity string to encrypt with.
 *   * identityLength
 *     * The length of the identity string.
 *   * publicParametersAsBinary
 *     * The BF-IBE public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_ibe_bonehFranklin_encrypt(
    BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *result,
    const char *const message, const size_t messageLength,
    const char *const identity, const size_t identityLength,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary, const AffinePoint *const precomputedPoints);

/**
 * ## Description
 *
 * Decrypts a previously encrypted message using the specified private key.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter holding the message in plaintext. If the return value is
 * CRYPTID_SUCCESS, then it will point to a zero-terminated string, that must be
 * destroyed by the caller. Initialization is done by this function.
 *   * ciphertextAsBinary
 *     * The ciphertext to decrypt.
 *   * privateKeyAsBinary
 *     * The private key to decrypt with.
 *   * publicParametersAsBinary
 *     * The BF-IBE public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_ibe_bonehFranklin_decrypt(
    char **result,
    const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        ciphertextAsBinary,
    const AffinePointAsBinary privateKeyAsBinary,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary, const AffinePoint *const precomputedPoints);

#endif

#endif