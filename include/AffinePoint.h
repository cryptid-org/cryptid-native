#ifndef AFFINEPOINT_H
#define AFFINEPOINT_H

#include "gmp.h"

#include "EllipticCurve.h"
#include "Status.h"

/**
 * Represents an affine point with coordinates.
 */
typedef struct AffinePoint
{
    /**
     * The x coordinate.
     */
    mpz_t x;

    /**
     * The y coordinate.
     */
    mpz_t y;
} AffinePoint;

/**
 * Returns a new AffinePoint initialized with the specified coordinates.
 * @param x the x coordinate
 * @param y the y coordinate
 * @return a new AffinePoint
 */
AffinePoint affine_init(mpz_t x, mpz_t y);

/**
 * Returns a new AffinePoint initialized with the specified long values.
 * @param xr the real part of the x coordinate
 * @param yr the real part of the y coordinate
 * @return a new AffinePoint
 */
AffinePoint affine_initLong(long x, long y);

/**
 * Frees an AffinePoint. After calling this function on an AffinePoint instance, that instance should not be used anymore.
 * @param affinePoint the AffinePoint to be destroyed
 */
void affine_destroy(AffinePoint affinePoint);

/**
 * Returns the infinity point.
 * @return the infinity point
 */
AffinePoint affine_infinity();

/**
 * Checks two AffinePoint instances for equality.
 * @param affinePoint1 an AffinePoint
 * @param affinePoint2 an AffinePoint
 * @return 1 if the two instances hold equal values, 0 otherwise
 */
int affine_isEquals(AffinePoint affinePoint1, AffinePoint affinePoint2);

/**
 * Checks if the specified affinePoint is the infinity point.
 * @param affinePoint the point to check
 * @return 1 if the specified point is the infinity point, 0 otherwise
 */
int affine_isInfinity(AffinePoint affinePoint);

Status affine_double(AffinePoint *result, AffinePoint affinePoint, EllipticCurve ellipticCurve);

/**
 * Adds two AffinePoints.
 *
 * Implementation of Algorithm 3.1 in [Intro-to-IBE].
 * @param result the result of the addition
 * @param affinePoint1 an AffinePoint
 * @param affinePoint2 an AffinePoint
 * @param ec the curve to operate over
 * @return 0 if everything went right, error otherwise
 */
Status affine_add(AffinePoint *result, AffinePoint affinePoint1, AffinePoint affinePoint2, EllipticCurve ellipticCurve);

/**
 * Multiplies an AffinePoint with a scalar.
 * @param result the result of the multiplication
 * @param s the scalar to multiply with
 * @param affinePoint the point to multiply
 * @param ec the elliptic curve to operate over
 * @return 0 if everything went right, error otherwise
 */
Status affine_multiply(AffinePoint *result, mpz_t s, AffinePoint affinePoint, EllipticCurve ellipticCurve);

Status affine_wNAFMultiply(AffinePoint *result, mpz_t s, AffinePoint affinePoint, EllipticCurve ellipticCurve);

int affine_isOnCurve(AffinePoint point, EllipticCurve ellipticCurve);

#endif
