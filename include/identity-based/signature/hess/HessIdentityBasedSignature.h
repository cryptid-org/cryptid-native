#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H

#ifdef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"

/**
 * Establishes a master secret and public parameters for a given security level. The master secret (as its name suggests)
 * should be kept secret, while the public parameters can be distributed among the clients.
 * @param masterSecret Out parameter which will hold the master secret. Must be mpz_init'd and mpz_clear'd by the caller.
 * @param publicParameters Pointer in which the public parameters will be stored. If the return value is CRYPTID_SUCCESS
 *                         then it will point to correctly filled HessIdentityBasedSignaturePublicParameters instance. Note, that the
 *                         q field must be mpz_init'd and thus mpz_clear'd be the caller.
 * @param securityLevel the desired security level
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_setup(mpz_t masterSecret, HessIdentityBasedSignaturePublicParameters* publicParameters, const SecurityLevel securityLevel);

/**
 * Extracts the private key corresponding to a given identity string.
 * @param result Out parameter holding the private key (which is an AffinePoint). On CRYPTID_SUCCESS, it must be destroyed by 
 *               the caller. Initialization is done by this function.
 * @param identity the identity string we're extracting the private key for
 * @param identityLength the length of the identity string
 * @param masterSecret the master secret corresponding to the public parameters
 * @param publicParameters the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_extract(AffinePoint* result, const char *const identity, const size_t identityLength, const mpz_t masterSecret,
                    const HessIdentityBasedSignaturePublicParameters publicParameters);

/**
 * Signs a message with the given identity string.
 * @param result Out parameter storing the signature. If the return value is CRYPTID_SUCCESS
 *               then it will point to a HessIdentityBasedSignatureSignature instance, that must be destroyed by the caller.
 *               Initialization is done by this function.
 * @param message the string to sign
 * @param messageLength the length of the message
 * @param identity the identity string to sign with
 * @param identityLength the length of the identity
 * @param privateKey the private key to sign with
 * @param publicParameters the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if everything went right
 */
CryptidStatus cryptid_ibs_hess_sign(HessIdentityBasedSignatureSignature *result, const char *const message, const size_t messageLength,
                    const char *const identity, const size_t identityLength, const AffinePoint privateKey, const HessIdentityBasedSignaturePublicParameters publicParameters);

/**
 * Verifies the signature to a previously signed message.
 * @param message the string which was signed
 * @param messageLength the length of the message
 * @param signature the digital signature of the message, which needs to be verified
 * @param identity the identity string the message was signed with
 * @param identityLength the length of the identity
 * @param publicParameters the Hess-IBS public parameters
 * @return CRYPTID_SUCCESS if the signature was valid
 */
CryptidStatus cryptid_ibs_hess_verify(const char *const message, const size_t messageLength, const HessIdentityBasedSignatureSignature signature,
                    const char *const identity, const size_t identityLength, const HessIdentityBasedSignaturePublicParameters publicParameters);

#endif

#endif