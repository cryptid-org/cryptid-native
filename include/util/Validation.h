#ifndef __CRYPTID_VALIDATION_H
#define __CRYPTID_VALIDATION_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/CipherTextTuple.h"
#include "identity-based/HashFunction.h"
#include "identity-based/PublicParameters.h"
#include "identity-based/Signature.h"


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
CryptidValidationResult validation_isPublicParametersValid(const PublicParameters publicParameters);

/**
 * Validates that the specified ciphertext is correct.
 * @param cipherTextTuple the ciphertext to check
 * @param order the field order of the elliptic curve field, we operating over
 * @return CRYPTID_VALIDATION_SUCCESS if the ciphertext is valid
 */
CryptidValidationResult validation_isCipherTextTupleValid(const CipherTextTuple cipherTextTuple, const mpz_t order);

/**
 * Validates that the specified signature is correct.
 * @param signature the signature to check
 * @param order the field order of the elliptic curve field, we operating over
 * @return CRYPTID_VALIDATION_SUCCESS if the signature is valid
 */ 
CryptidValidationResult validation_isSignatureValid(const Signature signature, const mpz_t order);

#endif
