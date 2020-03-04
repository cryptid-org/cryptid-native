#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H

#ifdef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE

#include "gmp.h"

#include "identity-based/signature/hess/HessIdentityBasedSignatureMasterSecretAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h"
#include "elliptic/AffinePoint.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"

/**
 * Establishes a master secret and public parameters for a given security level. The master secret (as its name suggests)
 * should be kept secret, while the public parameters can be distributed among the clients.
 * @param masterSecretAsBinary Out parameter which will hold the master secret.
 * @param publicParametersAsBinary Pointer in which the public parameters will be stored. If the return value is CRYPTID_SUCCESS
 *                         then it will point to correctly filled HessIdentityBasedSignaturePublicParameters instance
 * @param securityLevel the desired security level
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_setup(HessIdentityBasedSignatureMasterSecretAsBinary *masterSecretAsBinary, HessIdentityBasedSignaturePublicParametersAsBinary *publicParametersAsBinary, const SecurityLevel securityLevel);

/**
 * Extracts the private key corresponding to a given identity string.
 * @param result Out parameter holding the private key (which is an AffinePoint). On CRYPTID_SUCCESS, it must be destroyed by 
 *               the caller. Initialization is done by this function.
 * @param identity the identity string we're extracting the private key for
 * @param identityLength the length of the identity string
 * @param masterSecretAsBinary the master secret corresponding to the public parameters
 * @param publicParametersAsBinary the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_extract(AffinePointAsBinary *result, const char *const identity, const size_t identityLength, const HessIdentityBasedSignatureMasterSecretAsBinary masterSecretAsBinary, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary);

/**
 * Signs a message with the given identity string.
 * @param result Out parameter storing the signature. If the return value is CRYPTID_SUCCESS
 *               then it will point to a HessIdentityBasedSignatureSignatureAsBinary instance, that must be destroyed by the caller.
 *               Initialization is done by this function.
 * @param message the string to sign
 * @param messageLength the length of the message
 * @param identity the identity string to sign with
 * @param identityLength the length of the identity
 * @param privateKeyAsBinary the private key to sign with
 * @param publicParametersAsBinary the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_sign(HessIdentityBasedSignatureSignatureAsBinary *result, const char *const message, const size_t messageLength, const char *const identity, const size_t identityLength, const AffinePointAsBinary privateKeyAsBinary, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary);

/**
 * Verifies the signature to a previously signed message.
 * @param message the string which was signed
 * @param messageLength the length of the message
 * @param signatureAsBinary the digital signature of the message, which needs to be verified
 * @param identity the identity string the message was signed with
 * @param identityLength the length of the identity
 * @param publicParametersAsBinary the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if the signature was valid
 */
CryptidStatus cryptid_ibs_hess_verify(const char *const message, const size_t messageLength, const HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary, const char *const identity, const size_t identityLength, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary);

#endif

#endif