#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H

#ifdef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsString.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsString.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"

/**
 * Establishes a master secret and public parameters for a given security level. The master secret (as its name suggests)
 * should be kept secret, while the public parameters can be distributed among the clients.
 * @param masterSecret Out parameter which will hold the master secret.
 * @param publicParameters Pointer in which the public parameters will be stored. If the return value is CRYPTID_SUCCESS
 *                         then it will point to correctly filled HessIdentityBasedSignaturePublicParameters instance
 * @param masterSecretAsStringBase the base of the masterSecret string representation
 * @param securityLevel the desired security level
 * @param base the base of every other number's string representation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_setup(char **masterSecretAsString, HessIdentityBasedSignaturePublicParametersAsString *publicParametersAsString, const int masterSecretAsStringBase, const SecurityLevel securityLevel, const int base);

/**
 * Extracts the private key corresponding to a given identity string.
 * @param result Out parameter holding the private key (which is an AffinePoint). On CRYPTID_SUCCESS, it must be destroyed by 
 *               the caller. Initialization is done by this function.
 * @param identity the identity string we're extracting the private key for
 * @param identityLength the length of the identity string
 * @param masterSecretAsString the master secret corresponding to the public parameters
 * @param masterSecretAsStringBase the base of the masterSecret string representation
 * @param publicParametersAsString the Hess-IBS public parameters
 * @param base the base of every other number's string representation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_extract(AffinePointAsString *result, const char *const identity, const size_t identityLength, const char *const masterSecretAsString, const int masterSecretAsStringBase, const HessIdentityBasedSignaturePublicParametersAsString publicParametersAsString, const int base);

/**
 * Signs a message with the given identity string.
 * @param result Out parameter storing the signature. If the return value is CRYPTID_SUCCESS
 *               then it will point to a HessIdentityBasedSignatureSignatureAsString instance, that must be destroyed by the caller.
 *               Initialization is done by this function.
 * @param message the string to sign
 * @param messageLength the length of the message
 * @param identity the identity string to sign with
 * @param identityLength the length of the identity
 * @param privateKeyAsString the private key to sign with
 * @param publicParametersAsString the Hess-IBS public parameters
 * @param base the base of every other number's string representation
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_sign(HessIdentityBasedSignatureSignatureAsString *result, const char *const message, const size_t messageLength, const char *const identity, const size_t identityLength, const AffinePointAsString privateKeyAsString, const HessIdentityBasedSignaturePublicParametersAsString publicParametersAsString, const int base);

/**
 * Verifies the signature to a previously signed message.
 * @param message the string which was signed
 * @param messageLength the length of the message
 * @param signatureAsString the digital signature of the message, which needs to be verified
 * @param identity the identity string the message was signed with
 * @param identityLength the length of the identity
 * @param publicParametersAsString the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if the signature was valid
 */
CryptidStatus cryptid_ibs_hess_verify(const char *const message, const size_t messageLength, const HessIdentityBasedSignatureSignatureAsString signatureAsString, const char *const identity, const size_t identityLength, const HessIdentityBasedSignaturePublicParametersAsString publicParametersAsString);

#endif

#endif