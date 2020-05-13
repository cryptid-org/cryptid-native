#ifndef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H
#define __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE_H

#ifdef __CRYPTID_HESS_IDENTITY_BASED_SIGNATURE

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureMasterSecretAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h"
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
 * [HessIdentityBasedSignatureMasterSecretAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureMasterSecretAsBinary.h#HessIdentityBasedSignatureMasterSecretAsBinary)
 * instance, that must be destroyed by the caller. Initialization is done by
 * this function.
 *   * publicParametersAsBinary
 *     * Pointer in which the public parameters will be stored. If the return
 * value is CRYPTID_SUCCESS then it will point to correctly filled
 * [HessIdentityBasedSignaturePublicParametersAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h#HessIdentityBasedSignaturePublicParametersAsBinary)
 * instance.
 *   * securityLevel
 *     * The desired security level.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_ibs_hess_setup(
    HessIdentityBasedSignatureMasterSecretAsBinary *masterSecretAsBinary,
    HessIdentityBasedSignaturePublicParametersAsBinary
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
 *     * The Hess-IBS public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus cryptid_ibs_hess_extract(
    AffinePointAsBinary *result, const char *const identity,
    const size_t identityLength,
    const HessIdentityBasedSignatureMasterSecretAsBinary masterSecretAsBinary,
    const HessIdentityBasedSignaturePublicParametersAsBinary
        publicParametersAsBinary);

/**
 * ## Description
 *
 * Signs a message with the given identity string.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter storing the signature. If the return value is
 * CRYPTID_SUCCESS then it will point to a
 * [HessIdentityBasedSignatureSignatureAsBinary](codebase://identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h#HessIdentityBasedSignatureSignatureAsBinary)
 * instance, that must be destroyed by the caller. Initialization is done by
 * this function.
 *   * message
 *     * The string to sign.
 *   * messageLength
 *     * The length of the message.
 *   * identity
 *     * The identity string to sign with.
 *   * identityLength
 *     * The length of the identity.
 *   * privateKeyAsBinary
 *     * The private key to sign with.
 *   * publicParametersAsBinary
 *     * The Hess-IBS public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus
cryptid_ibs_hess_sign(HessIdentityBasedSignatureSignatureAsBinary *result,
                      const char *const message, const size_t messageLength,
                      const char *const identity, const size_t identityLength,
                      const AffinePointAsBinary privateKeyAsBinary,
                      const HessIdentityBasedSignaturePublicParametersAsBinary
                          publicParametersAsBinary);

/**
 * ## Description
 *
 * Verifies the signature to a previously signed message.
 *
 * ## Parameters
 *
 *   * message
 *     * The string which was signed.
 *   * messageLength
 *     * The length of the message.
 *   * signatureAsBinary
 *     * The digital signature of the message, which needs to be verified.
 *   * identity
 *     * The identity string the message was signed with.
 *   * identityLength
 *     * The length of the identity.
 *   * publicParametersAsBinary
 *     * The Hess-IBS public parameters.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if the signature was valid.
 */
CryptidStatus cryptid_ibs_hess_verify(
    const char *const message, const size_t messageLength,
    const HessIdentityBasedSignatureSignatureAsBinary signatureAsBinary,
    const char *const identity, const size_t identityLength,
    const HessIdentityBasedSignaturePublicParametersAsBinary
        publicParametersAsBinary);

#endif

#endif