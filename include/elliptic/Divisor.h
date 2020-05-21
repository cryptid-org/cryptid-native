#ifndef __CRYPTID_DIVISOR_H
#define __CRYPTID_DIVISOR_H

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Evaluates the divisor of a vertical line on a Type-1 elliptic curve.
 *
 * ## Parameters
 *
 *   * result
 *     * The resulting element of \f$F_p^2\f$ that is the divisor of the
 * vertical line going through \f$A\f$ evaluated at \f$B\f$.
 *   * a
 *     * A point in \f$E(F_p)\f$.
 *   * b
 *     * A point \f$E(F_p^2)\f$.
 *   * ec
 *     * The elliptic curve to operate on.
 */
void divisor_evaluateVertical(Complex *result, const AffinePoint a,
                              const ComplexAffinePoint b,
                              const EllipticCurve ec);

/**
 * ## Description
 *
 * Evaluates the divisor of a tangent on a Type-1 elliptic curve.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter: an element of \f$F_p^2\f$ that is the divisor of the
 * line tangent to \f$A\f$ evaluated at \f$B\f$. On CRYPTID_SUCCESS, this should
 * be destroyed by the caller.
 *   * a
 *     * A point in \f$E(F_p)\f$.
 *   * b
 *     * A point \f$E(F_p^2)\f$.
 *   * ec
 *     * The elliptic curve to operate on.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus divisor_evaluateTangent(Complex *result, const AffinePoint a,
                                      const ComplexAffinePoint b,
                                      const EllipticCurve ec);

/**
 * ## Description
 *
 * Evaluates the divisor of a line on a Type-1 elliptic curve.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter: an element of \f$F_p^2\f$ that is the divisor of the
 * line going through \f$A^{\prime}\f$ and \f$A^{\prime\prime}\f$ evaluated at
 * \f$B\f$. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 *   * a
 *     * A point in \f$E(F_p)\f$.
 *   * aprime
 *     * A point in \f$E(F_p)\f$.
 *   * b
 *     * A point in \f$E(F_p^2)\f$.
 *   * ec
 *     * The elliptic curve to operate on.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus divisor_evaluateLine(Complex *result, const AffinePoint a,
                                   const AffinePoint aprime,
                                   const ComplexAffinePoint b,
                                   const EllipticCurve ec);

#endif
