#ifndef __CRYPTID_AFFINEPOINT_H
#define __CRYPTID_AFFINEPOINT_H

#include "gmp.h"

#include "elliptic/EllipticCurve.h"
#include "util/Status.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * Represents an affine point with coordinates.
 */
typedef struct AffinePoint {
  /**
   * ## Description
   *
   * The \f$x\f$ coordinate.
   */
  mpz_t x;

  /**
   * ## Description
   *
   * The \f$y\f$ coordinate.
   */
  mpz_t y;
} AffinePoint;

/**
 * ## Description
 *
 * Initializes a new AffinePoint with the specified arbitrary precision
 * coordinates.
 *
 * ## Parameters
 *
 *   * affinePointOutput
 *     * The AffinePoint to be initialized.
 *   * x
 *     * The \f$x\f$ coordinate.
 *   * y
 *     * The \f$y\f$ coordinate.
 */
void affine_init(AffinePoint *affinePointOutput, const mpz_t x, const mpz_t y);

/**
 * ## Description
 *
 * Initializes a new AffinePoint with the specified long values.
 *
 * ## Parameters
 *
 *   * affinePointOutput
 *     * The AffinePoint to be initialized.
 *   * x
 *     * The \f$x\f$ coordinate.
 *   * y
 *     * The \f$y\f$ coordinate.
 */
void affine_initLong(AffinePoint *affinePointOutput, const long x,
                     const long y);

/**
 * ## Description
 *
 * Frees an AffinePoint. After calling this function on an AffinePoint instance,
 * that instance should not be used anymore.
 *
 * ## Parameters
 *
 *   * affinePoint
 *     * The AffinePoint to be destroyed.
 */
void affine_destroy(AffinePoint affinePoint);

/**
 * ## Description
 *
 * Returns the infinity point.
 *
 * ## Return Value
 *
 * The infinity point.
 */
AffinePoint affine_infinity(void);

/**
 * ## Description
 *
 * Checks two AffinePoint instances for equality.
 *
 * ## Parameters
 *
 *   * affinePoint1
 *     * An AffinePoint.
 *   * affinePoint2
 *     * An AffinePoint.
 *
 * ## Return Value
 *
 * 1 if the two instances hold equal values, 0 otherwise.
 */
int affine_isEquals(const AffinePoint affinePoint1,
                    const AffinePoint affinePoint2);

/**
 * ## Description
 *
 * Checks if the specified AffinePoint is the infinity point.
 *
 * ## Parameters
 *
 *   * affinePoint
 *     * The point to check.
 *
 * ## Return Value
 *
 * 1 if the specified point is the infinity point, 0 otherwise.
 */
int affine_isInfinity(const AffinePoint affinePoint);

/**
 * ## Description
 *
 * Doubles (adds to itself) the specified AffinePoint.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the operation. On CRYPTID_SUCCESS, this should be
 * destroyed by the caller.
 *   * affinePoint
 *     * The point to double.
 *   * ellipticCurve
 *     * The elliptic curve to operate over.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right, error otherwise
 */
CryptidStatus affine_double(AffinePoint *result, const AffinePoint affinePoint,
                            const EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Adds two AffinePoints.
 * Implementation of Algorithm 3.1 in [Intro-to-IBE].
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the addition. On CRYPTID_SUCCESS, this should be
 * destroyed by the caller.
 *   * affinePoint1
 *     * An AffinePoint.
 *   * affinePoint2
 *     * An AffinePoint.
 *   * ellipticCurve
 *     * The curve to operate over.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right, error otherwise.
 */
CryptidStatus affine_add(AffinePoint *result, const AffinePoint affinePoint1,
                         const AffinePoint affinePoint2,
                         const EllipticCurve ellipticCurve);

CryptidStatus affine_precompute(AffinePoint **result, const AffinePoint affinePoint, const int rounds, const EllipticCurve ellipticCurve);

CryptidStatus affine_multiply_with_precomputedPoints(AffinePoint *result,
                                     const mpz_t s,
                                     const EllipticCurve ellipticCurve,
                                     const AffinePoint *const precomputedPoints);

/**
 * ## Description
 *
 * Multiplies an AffinePoint with a scalar.
 * Implementation note: Uses the windowed-NAF algorithm.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the multiplication. On CRYPTID_SUCCESS, this should be
 * destroyed by the caller.
 *   * affinePoint
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
CryptidStatus affine_wNAFMultiply(AffinePoint *result,
                                  const AffinePoint affinePoint, const mpz_t s,
                                  const EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Checks if the specified point is on the passed curve (satisfies the curve
 * equation).
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
int affine_isOnCurve(const AffinePoint point,
                     const EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Validates that the affine point is between 0 and order-1.
 *
 * ## Parameters
 *
 *   * affinePoint
 *     * An AffinePoint.
 *   * ellipticCurve
 *     * The elliptic curve field, we are operating over.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the point is valid, CRYPTID_VALIDATION_FAILURE
 * otherwise.
 */
CryptidValidationResult affine_isValid(const AffinePoint affinePoint,
                                       const EllipticCurve ellipticCurve);

#endif
