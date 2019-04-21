#ifndef COMPLEXAFFINEPOINT_H
#define COMPLEXAFFINEPOINT_H

#include "gmp.h"

#include "Complex.h"
#include "EllipticCurve.h"
#include "Status.h"

/**
 * Represents an affine point with Complex coordinates.
 */
typedef struct ComplexAffinePoint
{
    /**
     * The x coordinate.
     */
    Complex x;

    /**
     * The y coordinate.
     */
    Complex y;
} ComplexAffinePoint;

/**
 * Returns a new ComplexAffinePoint initialized with the specified coordinates.
 * @param x the x coordinate
 * @param y the y coordinate
 * @return a new ComplexAffinePoint
 */
ComplexAffinePoint complexAffine_init(Complex x, Complex y);

/**
 * Returns a new ComplexAffinePoint initialized with the specified long values.
 * @param xr the real part of the x coordinate
 * @param xi the imaginary part of the x coordinate
 * @param yr the real part of the y coordinate
 * @param yi the imaginary part of the y coordinate
 * @return a new ComplexAffinePoint
 */
ComplexAffinePoint complexAffine_initLong(long xr, long xi, long yr, long yi);

/**
 * Frees a ComplexAffinePoint. After calling this function on a ComplexAffinePoint instance, that instance should not be used anymore.
 * @param complexAffinePoint the ComplexAffinePoint to be destroyed
 */
void complexAffine_destroy(ComplexAffinePoint complexAffinePoint);

/**
 * Returns the infinity point.
 * @return the infinity point
 */
ComplexAffinePoint complexAffine_infinity();

/**
 * Checks two ComplexAffinePoint instances for equality.
 * @param complexAffinePoint1 an ComplexAffinePoint
 * @param complexAffinePoint2 an ComplexAffinePoint
 * @return 1 if the two instances hold equal values, 0 otherwise
 */
int complexAffine_isEquals(ComplexAffinePoint complexAffinePoint1, ComplexAffinePoint complexAffinePoint2);

/**
 * Checks if the specified complexAffinePoint is the infinity point.
 * @param complexAffinePoint the point to check
 * @return 1 if the specified point is the infinity point, 0 otherwise
 */
int complexAffine_isInfinity(ComplexAffinePoint complexAffinePoint);

Status complexAffine_double(ComplexAffinePoint *result, ComplexAffinePoint complexAffinePoint, EllipticCurve ellipticCurve);

/**
 * Adds two ComplexAffinePoints.
 *
 * Implementation of Algorithm 3.1 in [Intro-to-IBE].
 * @param result the result of the addition
 * @param complexAffinePoint1 a ComplexAffinePoint
 * @param complexAffinePoint2 a ComplexAffinePoint
 * @param ec the curve to operate over
 * @return 0 if everything went right, error otherwise
 */
Status complexAffine_add(ComplexAffinePoint *result, ComplexAffinePoint complexAffinePoint1, ComplexAffinePoint complexAffinePoint2, EllipticCurve ellipticCurve);

/**
 * Multiplies a ComplexAffinePoint with a scalar.
 * @param result the result of the multiplication
 * @param s the scalar to multiply with
 * @param complexAffinePoint the point to multiply
 * @param ec the elliptic curve to operate over
 * @return 0 if everything went right, error otherwise
 */
Status complexAffine_multiply(ComplexAffinePoint *result, mpz_t s, ComplexAffinePoint complexAffinePoint, EllipticCurve ellipticCurve);

int complexAffine_isOnCurve(ComplexAffinePoint point, EllipticCurve ellipticCurve);

#endif
