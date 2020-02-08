#ifndef __CRYPTID_AFFINEPOINT_H
#define __CRYPTID_AFFINEPOINT_H

#include "gmp.h"

#include "elliptic/EllipticCurve.h"
#include "util/Status.h"

/**
 * We have multiple implemetations of the affine-scalar multiplication. This macro
 * defines which one to use.
 */
#define AFFINE_MULTIPLY_IMPL affine_wNAFMultiply


/**
 * Represents an affine point with coordinates.
 */
typedef struct AffinePoint
{
    /**
     * The \f$x\f$ coordinate.
     */
    mpz_t x;

    /**
     * The \f$y\f$ coordinate.
     */
    mpz_t y;
} AffinePoint;

/**
 * Initializes a new AffinePoint with the specified arbitrary precision coordinates.
 * @param affinePointOutput the AffinePoint to be initialized
 * @param x the \f$x\f$ coordinate
 * @param y the \f$y\f$ coordinate
 */
void affine_init(AffinePoint *affinePointOutput, const mpz_t x, const mpz_t y);

/**
 * Initializes a new AffinePoint with the specified long values.
 * @param affinePointOutput the AffinePoint to be initialized
 * @param x the \f$x\f$ coordinate
 * @param y the \f$y\f$ coordinate
 */
void affine_initLong(AffinePoint *affinePointOutput, const long x, const long y);

/**
 * Frees an AffinePoint. After calling this function on an AffinePoint instance,
 * that instance should not be used anymore.
 * @param affinePoint the AffinePoint to be destroyed
 */
void affine_destroy(AffinePoint affinePoint);

/**
 * Returns the infinity point.
 * @return the infinity point
 */
AffinePoint affine_infinity(void);

/**
 * Checks two AffinePoint instances for equality.
 * @param affinePoint1 an AffinePoint
 * @param affinePoint2 an AffinePoint
 * @return 1 if the two instances hold equal values, 0 otherwise
 */
int affine_isEquals(const AffinePoint affinePoint1, const AffinePoint affinePoint2);

/**
 * Checks if the specified AffinePoint is the infinity point.
 * @param affinePoint the point to check
 * @return 1 if the specified point is the infinity point, 0 otherwise
 */
int affine_isInfinity(const AffinePoint affinePoint);

/**
 * Doubles (adds to itself) the specified AffinePoint.
 * @param result The result of the operation. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 * @param affinePoint the point to double
 * @param ellipticCurve the elliptic curve to operate over
 * @result CRYPTID_SUCCESS if everything went right, error otherwise
 */
CryptidStatus affine_double(AffinePoint *result, const AffinePoint affinePoint, const EllipticCurve ellipticCurve);

/**
 * Adds two AffinePoints.
 *
 * Implementation of Algorithm 3.1 in [Intro-to-IBE].
 * @param result The result of the addition. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 * @param affinePoint1 an AffinePoint
 * @param affinePoint2 an AffinePoint
 * @param ellipticCurve the curve to operate over
 * @return CRYPTID_SUCCESS if everything went right, error otherwise
 */
CryptidStatus affine_add(AffinePoint *result, const AffinePoint affinePoint1, const AffinePoint affinePoint2, const EllipticCurve ellipticCurve);

/**
 * Multiplies an AffinePoint with a scalar.
 * 
 * Implementation note: Uses the double-and-add algorithm.
 * @param result The result of the multiplication. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 * @param affinePoint the point to multiply
 * @param s the scalar to multiply with
 * @param ellipticCurve the elliptic curve to operate over
 * @return CRYPTID_SUCCESS if everything went right, error otherwise
 */
CryptidStatus affine_multiply(AffinePoint *result, const AffinePoint affinePoint, const mpz_t s, const EllipticCurve ellipticCurve);

/**
 * Multiplies an AffinePoint with a scalar.
 * 
 * Implementation note: Uses the windowed-NAF algorithm.
 * @param result The result of the multiplication. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 * @param affinePoint the point to multiply
 * @param s the scalar to multiply with
 * @param ellipticCurve the elliptic curve to operate over
 * @return CRYPTID_SUCCESS if everything went right, error otherwise
 */
CryptidStatus affine_wNAFMultiply(AffinePoint *result, const AffinePoint affinePoint, const mpz_t s, const EllipticCurve ellipticCurve);

/**
 * Checks if the specified point is on the passed curve (satisfies the curve equation).
 * @param point the point to check
 * @param ellipticCurve the elliptic curve
 * @return 1 if the point satisfies the curve equation, 0 otherwise
 */
int affine_isOnCurve(const AffinePoint point, const EllipticCurve ellipticCurve);

#endif
