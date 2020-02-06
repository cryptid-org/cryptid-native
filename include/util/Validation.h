#ifndef __CRYPTID_VALIDATION_H
#define __CRYPTID_VALIDATION_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/HashFunction.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCipherText.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParameters.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignature.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParameters.h"


/**
 * Enumeration of the possible results of a validation process.
 */
typedef enum
{
    /*
     * Flipped values match boolean semantics better.
     */ 
    CRYPTID_VALIDATION_SUCCESS = 1,
    CRYPTID_VALIDATION_FAILURE = 0
} CryptidValidationResult;


/**
 * Check whether \f$p\f$ is a probable prime.
 * @param p the number to check
 * @result CRYPTID_VALIDATION_SUCCESS if \f$p\f$ is a probable prime
 */
CryptidValidationResult validation_isProbablePrime(const mpz_t p);

/**
 * Validates that the passed HashFunction struct is correct.
 * @param hashFunction the struct to check
 * @result CRYPTID_VALIDATION_SUCCESS if the struct is valid
 */
CryptidValidationResult validation_isHashFunctionValid(const HashFunction hashFunction);

/**
 * Validates that the affine point is between 0 and order-1
 * @param affinePoint an AffinePoint
 * @param order the field order of the elliptic curve field, we are operating over
 * @return CRYPTID_VALIDATION_SUCCESS if the point is valid, CRYPTID_VALIDATION_FAILURE otherwise
 */
CryptidValidationResult validation_isAffinePointValid(const AffinePoint affinePoint, const mpz_t order);

/**
 * Validates that the specified EllipticCurve instance is a Type-1 elliptic curve.
 * @param ellipticCurve the struct to check
 * @return CRYPTID_VALIDATION_SUCCESS if the struct is valid
 */
CryptidValidationResult validation_isTypeOneEllipticCurve(const EllipticCurve ellipticCurve);

/**
 * Validates that the specified public parameters are correct.
 * @param publicParameters the parameters to check
 * @return CRYPTID_VALIDATION_SUCCESS if the parameters are valid
 */
CryptidValidationResult validation_isBonehFranklinIdentityBasedEncryptionPublicParametersValid(const BonehFranklinIdentityBasedEncryptionPublicParameters publicParameters);

/**
 * Validates that the specified public parameters are correct.
 * @param publicParameters the parameters to check
 * @return CRYPTID_VALIDATION_SUCCESS if the parameters are valid
 */
CryptidValidationResult validation_isHessIdentityBasedSignaturePublicParametersValid(const HessIdentityBasedSignaturePublicParameters publicParameters);

/**
 * Validates that the specified ciphertext is correct.
 * @param bonehFranklinIdentityBasedEncryptionCipherText the ciphertext to check
 * @param order the field order of the elliptic curve field, we operating over
 * @return CRYPTID_VALIDATION_SUCCESS if the ciphertext is valid
 */
CryptidValidationResult validation_isBonehFranklinIdentityBasedEncryptionCipherTextValid(const BonehFranklinIdentityBasedEncryptionCipherText bonehFranklinIdentityBasedEncryptionCipherText, const mpz_t order);

/**
 * Validates that the specified signature is correct.
 * @param signature the signature to check
 * @param order the field order of the elliptic curve field, we operating over
 * @return CRYPTID_VALIDATION_SUCCESS if the signature is valid
 */ 
CryptidValidationResult validation_isHessIdentityBasedSignatureSignatureValid(const HessIdentityBasedSignatureSignature signature, const mpz_t order);

#endif
