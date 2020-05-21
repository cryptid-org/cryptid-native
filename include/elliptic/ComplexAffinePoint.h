#ifndef __CRYPTID_COMPLEXAFFINEPOINT_H
#define __CRYPTID_COMPLEXAFFINEPOINT_H

#include "gmp.h"

#include "complex/Complex.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Represents an affine point with Complex coordinates.
 */
typedef struct ComplexAffinePoint {
  /**
   * ## Description
   *
   * The x coordinate.
   */
  Complex x;

  /**
   * ## Description
   *
   * The y coordinate.
   */
  Complex y;
} ComplexAffinePoint;

/**
 * ## Description
 *
 * Initializes a new ComplexAffinePoint with the specified coordinates.
 *
 * ## Parameters
 *
 *   * complexAffinePointOutput
 *     * The ComplexAffinePoint to be initialized.
 *   * x
 *     * The \f$x\f$ coordinate.
 *   * y
 *     * The \f$y\f$ coordinate.
 */
void complexAffine_init(ComplexAffinePoint *complexAffinePointOutput,
                        const Complex x, const Complex y);

/**
 * ## Description
 *
 * Initializes a new ComplexAffinePoint initialized with the specified long values.
 *
 * ## Parameters
 *
 *   * complexAffinePointOutput
 *     * The ComplexAffinePoint to be initialized.
 *   * xr
 *     * The real part of the \f$x\f$ coordinate.
 *   * xi
 *     * the imaginary part of the \f$x\f$ coordinate.
 *   * yr
 *     * The real part of the \f$y\f$ coordinate.
 *   * yi
 *     * The imaginary part of the \f$y\f$ coordinate.
 */
void complexAffine_initLong(ComplexAffinePoint *complexAffinePointOutput,
                            const long xr, const long xi, const long yr,
                            const long yi);

/**
 * ## Description
 *
 * Frees a ComplexAffinePoint. After calling this function on a ComplexAffinePoint instance, that instance should not be used anymore.
 *
 * ## Parameters
 *
 *   * complexAffinePoint
 *     * The ComplexAffinePoint to be destroyed.
 */
void complexAffine_destroy(ComplexAffinePoint complexAffinePoint);

/**
 * ## Description
 *
 * Returns the infinity point.
 *
 * ## Return Value
 *
 * The infinity point.
 */
ComplexAffinePoint complexAffine_infinity(void);

/**
 * ## Description
 *
 * Checks two ComplexAffinePoint instances for equality.
 *
 * ## Parameters
 *
 *   * complexAffinePoint1
 *     * An ComplexAffinePoint.
 *   * complexAffinePoint2
 *     * An ComplexAffinePoint.
 *
 * ## Return Value
 *
 * 1 if the two instances hold equal values, 0 otherwise.
 */
int complexAffine_isEquals(const ComplexAffinePoint complexAffinePoint1,
                           const ComplexAffinePoint complexAffinePoint2);

/**
 * ## Description
 *
 * Checks if the specified complexAffinePoint is the infinity point.
 *
 * ## Parameters
 *
 *   * complexAffinePoint
 *     * The point to check.
 *
 * ## Return Value
 *
 * 1 if the specified point is the infinity point, 0 otherwise.
 */
int complexAffine_isInfinity(const ComplexAffinePoint complexAffinePoint);

/**
 * ## Description
 *
 * Doubles (adds to itself) the specified ComplexAffinePoint.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the operation. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 *   * complexAffinePoint
 *     * The point to double.
 *   * ellipticCurve
 *     * The elliptic curve to operate over.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right, error otherwise.
 */
CryptidStatus complexAffine_double(ComplexAffinePoint *result,
                                   const ComplexAffinePoint complexAffinePoint,
                                   const EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Adds two ComplexAffinePoints.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the addition. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 *   * complexAffinePoint1
 *     * A ComplexAffinePoint.
 *   * complexAffinePoint2
 *     * A ComplexAffinePoint.
 *   * ellipticCurve
 *     * The curve to operate over.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right, error otherwise.
 */
CryptidStatus complexAffine_add(ComplexAffinePoint *result,
                                const ComplexAffinePoint complexAffinePoint1,
                                const ComplexAffinePoint complexAffinePoint2,
                                const EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Multiplies a ComplexAffinePoint with a scalar.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the multiplication. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 *   * complexAffinePoint
 *     * The point to multiply.
 *   * s
 *     * The scalar to multiply with.
 *   * ellipticCurve
 *     * The elliptic curve to operate over.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right, error otherwise.
 */
CryptidStatus
complexAffine_multiply(ComplexAffinePoint *result,
                       const ComplexAffinePoint complexAffinePoint,
                       const mpz_t s, const EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Checks if the specified point is on the passed curve (satisfies the curve equation).
 *
 * ## Parameters
 *
 *   * point
 *     * The point to check.
 *   * ellipticCurve
 *     * The elliptic curve.
 *
 * ## Return Value
 *
 * 1 if the point satisfies the curve equation, 0 otherwise.
 */
int complexAffine_isOnCurve(const ComplexAffinePoint point,
                            const EllipticCurve ellipticCurve);

#endif
