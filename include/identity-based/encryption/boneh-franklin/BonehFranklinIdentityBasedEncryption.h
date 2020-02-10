#ifndef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_H
#define __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION_H

#ifdef __CRYPTID_BONEH_FRANKLIN_IDENTITY_BASED_ENCRYPTION

#include "gmp.h"

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h"
#include "elliptic/AffinePoint.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"


/**
 * Establishes a master secret and public parameters for a given security level. The master secret (as its name suggests)
 * should be kept secret, while the public parameters can be distributed among the clients.
 * @param masterSecretAsBinary Out parameter which will hold the master secret.
 * @param publicParametersAsBinary Pointer in which the public parameters will be stored. If the return value is CRYPTID_SUCCESS
 *                         then it will point to correctly filled BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary instance.
 * @param securityLevel the desired security level
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_setup(BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary *masterSecretAsBinary, BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary* publicParametersAsBinary, const SecurityLevel securityLevel);

/**
 * Extracts the private key corresponding to a given identity string.
 * @param result Out parameter holding the private key (which is an AffinePoint). On CRYPTID_SUCCESS, it must be destroyed by 
 *               the caller. Initialization is done by this function.
 * @param identity the identity string we're extracting the private key for
 * @param identityLength the length of the identity string
 * @param masterSecretAsBinary the master secret corresponding to the public parameters
 * @param publicParametersAsBinary the BF-IBE public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_extract(AffinePointAsBinary *result, const char *const identity, const size_t identityLength, const BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretAsBinary, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

/**
 * Encrypts a message with the given identity string.
 * @param result Out parameter storing the ciphertext. If the return value is CRYPTID_SUCCESS
 *               then it will point to an BonehFranklinIdentityBasedEncryptionCiphertextAsBinary instance, that must be destroyed by the caller.
 *               Initialization is done by this function.
 * @param message the string to encrypt
 * @param messageLength the length of the message
 * @param identity the identity string to encrypt with
 * @param identityLength the length of the identity
 * @param publicParametersAsBinary the BF-IBE public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_encrypt(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *result, const char *const message, const size_t messageLength, const char *const identity, const size_t identityLength, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

/**
 * Decrypts a previously encrypted message using the specified private key.
 * @param result Out parameter holding the message in plaintext. If the return value is CRYPTID_SUCCESS
 *               then it will point to a zero-terminated string, that must be destroyed by the caller. 
 * @param ciphertextAsBinary the ciphertext to decrypt
 * @param privateKeyAsBinary the private key to decrypt with
 * @param publicParametersAsBinary the BF-IBE public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibe_bonehFranklin_decrypt(char **result, const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertextAsBinary, const AffinePointAsBinary privateKeyAsBinary, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersAsBinary);

#endif

#endif