#ifndef __CRYPTID_COMPLEXAFFINEPOINT_H
#define __CRYPTID_COMPLEXAFFINEPOINT_H

#include "gmp.h"

#include "complex/Complex.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"


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
 * @param x the \f$x\f$ coordinate
 * @param y the \f$y\f$ coordinate
 * @return a new ComplexAffinePoint
 */
ComplexAffinePoint complexAffine_init(const Complex x, const Complex y);

/**
 * Returns a new ComplexAffinePoint initialized with the specified long values.
 * @param xr the real part of the \f$x\f$ coordinate
 * @param xi the imaginary part of the \f$x\f$ coordinate
 * @param yr the real part of the \f$y\f$ coordinate
 * @param yi the imaginary part of the \f$y\f$ coordinate
 * @return a new ComplexAffinePoint
 */
ComplexAffinePoint complexAffine_initLong(const long xr, const long xi, const long yr, const long yi);

/**
 * Frees a ComplexAffinePoint. After calling this function on a ComplexAffinePoint instance, 
 * that instance should not be used anymore.
 * @param complexAffinePoint the ComplexAffinePoint to be destroyed
 */
void complexAffine_destroy(ComplexAffinePoint complexAffinePoint);

/**
 * Returns the infinity point.
 * @return the infinity point
 */
ComplexAffinePoint complexAffine_infinity(void);

/**
 * Checks two ComplexAffinePoint instances for equality.
 * @param complexAffinePoint1 an ComplexAffinePoint
 * @param complexAffinePoint2 an ComplexAffinePoint
 * @return 1 if the two instances hold equal values, 0 otherwise
 */
int complexAffine_isEquals(const ComplexAffinePoint complexAffinePoint1, const ComplexAffinePoint complexAffinePoint2);

/**
 * Checks if the specified complexAffinePoint is the infinity point.
 * @param complexAffinePoint the point to check
 * @return 1 if the specified point is the infinity point, 0 otherwise
 */
int complexAffine_isInfinity(const ComplexAffinePoint complexAffinePoint);

/**
 * Doubles (adds to itself) the specified ComplexAffinePoint.
 * @param result the result of the operation
 * @param complexAffinePoint the point to double
 * @param ellipticCurve the elliptic curve to operate over
 * @result SUCCESS if everything went right, error otherwise
 * */
Status complexAffine_double(ComplexAffinePoint *result, const ComplexAffinePoint complexAffinePoint,
                            const EllipticCurve ellipticCurve);

/**
 * Adds two ComplexAffinePoints.
 * @param result the result of the addition
 * @param complexAffinePoint1 a ComplexAffinePoint
 * @param complexAffinePoint2 a ComplexAffinePoint
 * @param ec the curve to operate over
 * @return SUCCESS if everything went right, error otherwise
 */
Status complexAffine_add(ComplexAffinePoint *result, const ComplexAffinePoint complexAffinePoint1, 
                         const ComplexAffinePoint complexAffinePoint2, const EllipticCurve ellipticCurve);

/**
 * Multiplies a ComplexAffinePoint with a scalar.
 * @param result the result of the multiplication
 * @param s the scalar to multiply with
 * @param complexAffinePoint the point to multiply
 * @param ec the elliptic curve to operate over
 * @return SUCCESS if everything went right, error otherwise
 */
Status complexAffine_multiply(ComplexAffinePoint *result, const mpz_t s, const ComplexAffinePoint complexAffinePoint,
                              const EllipticCurve ellipticCurve);

/**
 * Checks if the specified point is on the passed curve (satisfies the curve equation).
 * @param point the point to check
 * @param ellipticCurve the elliptic curve
 * @return 1 if the point satisfies the curve equation, 0 otherwise
 */
int complexAffine_isOnCurve(const ComplexAffinePoint point, const EllipticCurve ellipticCurve);

#endif
