#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_H

#ifdef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION

#include "gmp.h"

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsString.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsString.h"
#include "elliptic/AffinePoint.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"


/**
 * Establishes a master secret and public parameters for a given security level. The master secret (as its name suggests)
 * should be kept secret, while the public parameters can be distributed among the clients.
 * @param masterSecretAsString Out parameter which will hold the master secret.
 * @param publicParametersAsString Pointer in which the public parameters will be stored. If the return value is CRYPTID_SUCCESS
 *                         then it will point to correctly filled BonehFranklinIdentityBasedEncryptionPublicParametersAsString instance.
 * @param masterSecretAsStringBase the base of the masterSecret string representation
 * @param securityLevel the desired security level
 * @param base the base of every other number's string representation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_setup(char **masterSecretAsString, BonehFranklinIdentityBasedEncryptionPublicParametersAsString* publicParametersAsString, const int masterSecretAsStringBase, const SecurityLevel securityLevel, const int base);

/**
 * Extracts the private key corresponding to a given identity string.
 * @param result Out parameter holding the private key (which is an AffinePoint). On CRYPTID_SUCCESS, it must be destroyed by 
 *               the caller. Initialization is done by this function.
 * @param identity the identity string we're extracting the private key for
 * @param identityLength the length of the identity string
 * @param masterSecretAsString the master secret corresponding to the public parameters
 * @param masterSecretAsStringBase the base of the masterSecret string representation
 * @param publicParametersAsString the BF-IBE public parameters
 * @param base the base of every other number's string representation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_extract(AffinePointAsString *result, const char *const identity, const size_t identityLength, const char *const masterSecretAsString, const int masterSecretAsStringBase, const BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString, const int base);

/**
 * Encrypts a message with the given identity string.
 * @param result Out parameter storing the ciphertext. If the return value is CRYPTID_SUCCESS
 *               then it will point to an BonehFranklinIdentityBasedEncryptionCiphertextAsString instance, that must be destroyed by the caller.
 *               Initialization is done by this function.
 * @param message the string to encrypt
 * @param messageLength the length of the message
 * @param identity the identity string to encrypt with
 * @param identityLength the length of the identity
 * @param publicParametersAsString the BF-IBE public parameters
 * @param base the base of every other number's string representation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_encrypt(BonehFranklinIdentityBasedEncryptionCiphertextAsString *result, const char *const message, const size_t messageLength, const char *const identity, const size_t identityLength, const BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString, const int base);

/**
 * Decrypts a previously encrypted message using the specified private key.
 * @param result Out parameter holding the message in plaintext. If the return value is CRYPTID_SUCCESS
 *               then it will point to a zero-terminated string, that must be destroyed by the caller. 
 * @param ciphertextAsString the ciphertext to decrypt
 * @param privateKeyAsString the private key to decrypt with
 * @param publicParametersAsString the BF-IBE public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_decrypt(char **result, const BonehFranklinIdentityBasedEncryptionCiphertextAsString ciphertextAsString, const AffinePointAsString privateKeyAsString, const BonehFranklinIdentityBasedEncryptionPublicParametersAsString publicParametersAsString);

#endif

#endif