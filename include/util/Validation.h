#ifndef __CRYPTID_VALIDATION_H
#define __CRYPTID_VALIDATION_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/CipherTextTuple.h"
#include "identity-based/HashFunction.h"
#include "identity-based/PublicParameters.h"


/**
 * Enumeration of the possible results of a validation process.
 */
typedef enum
{
    /*
     * Flipped values match boolean semantics better.
     */ 
    VALIDATION_SUCCESS = 1,
    VALIDATION_FAILURE = 0
} ValidationResult;


/**
 * Check whether \f$p\f$ is a probable prime.
 * @param p the number to check
 * @result VALIDATION_SUCCESS if \f$p\f$ is a probable prime
 */
ValidationResult validation_isProbablePrime(const mpz_t p);

/**
 * Validates that the passed HashFunction struct is correct.
 * @param hashFunction the struct to check
 * @result VALIDATION_SUCCESS if the struct is valid
 */
ValidationResult validation_isHashFunctionValid(const HashFunction hashFunction);

/**
 * Validates that the affine point is between 0 and order-1
 * @param affinePoint an AffinePoint
 * @param order the field order of the elliptic curve field, we are operating over
 * @return VALIDATION_SUCCESS if the point is valid, VALIDATION_FAILURE otherwise
 */
ValidationResult validation_isAffinePointValid(const AffinePoint affinePoint, const mpz_t order);

/**
 * Validates that the specified EllipticCurve instance is a Type-1 elliptic curve.
 * @param ellipticCurve the struct to check
 * @return VALIDATION_SUCCESS if the struct is valid
 */
ValidationResult validation_isTypeOneEllipticCurve(const EllipticCurve ellipticCurve);

/**
 * Validates that the specified public parameters are correct.
 * @param publicParameters the parameters to check
 * @return VALIDATION_SUCCESS if the parameters are valid
 */
ValidationResult validation_isPublicParametersValid(const PublicParameters publicParameters);

/**
 * Validates that the specified ciphertext is correct.
 * @param cipherTextTuple the ciphertext to check
 * @param order the field order of the elliptic curve field, we operating over
 * @return VALIDATION_SUCCESS if the ciphertext is valid
 */
ValidationResult validation_isCipherTextTupleValid(const CipherTextTuple cipherTextTuple, const mpz_t order);

#endif
