#ifndef __CRYPTID_VALIDATION_H
#define __CRYPTID_VALIDATION_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/CipherTextTuple.h"
#include "identity-based/HashFunction.h"
#include "identity-based/PublicParameters.h"


typedef enum
{
    /*
     * Flipped values match boolean semantics better.
     */ 
    VALIDATION_SUCCESS = 1,
    VALIDATION_FAILURE = 0
} ValidationResult;

ValidationResult validation_isProbablePrime(mpz_t p);

ValidationResult validation_isHashFunctionValid(HashFunction hashFunction);

/**
 * Validates that the affine point is between 0 and order-1
 * @param affinePoint an AffinePoint
 * @param order the field order of the elliptic curve, we operate over
 * @return VALIDATION_SUCCESS if the point is valid, VALIDATION_FAILURE otherwise
 */
ValidationResult validation_isAffinePointValid(AffinePoint affinePoint, mpz_t order);

ValidationResult validation_isTypeOneEllipticCurve(EllipticCurve ellipticCurve);

ValidationResult validation_isPublicParametersValid(PublicParameters publicParameters);

ValidationResult validation_isCipherTextTupleValid(CipherTextTuple cipherTextTuple, mpz_t order);

#endif
