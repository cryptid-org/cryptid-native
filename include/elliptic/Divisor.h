#ifndef __CRYPTID_DIVISOR_H
#define __CRYPTID_DIVISOR_H

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"


/**
 * Evaluates the divisor of a vertical line on a Type-1 elliptic curve.
 * @param ec the elliptic curve to operate on
 * @param a a point in \f$E(F_p)\f$
 * @param b a point \f$E(F_p^2)\f$
 * @return an element of \f$F_p^2\f$ that is the divisor of the vertical line going through \f$A\f$ evaluated at \f$B\f$
 */
Complex divisor_evaluateVertical(const EllipticCurve ec, const AffinePoint a, const ComplexAffinePoint b);

/**
 * Evaluates the divisor of a tangent on a Type-1 elliptic curve.
 * @param result pointer to an element of \f$F_p^2\f$ that is the divisor of the line tangent to \f$A\f$ 
 *               evaluated at \f$B\f$
 * @param ec the elliptic curve to operate on
 * @param a a point in \f$E(F_p)\f$
 * @param b a point \f$E(F_p^2)\f$
 * @result SUCCESS if everything went right
 */
Status divisor_evaluateTangent(Complex* result, const EllipticCurve ec, const AffinePoint a, const ComplexAffinePoint b);

/**
 * Evaluates the divisor of a line on a Type-1 elliptic curve.
 * @param result pointer an element of \f$F_p^2\f$ that is the divisor of the line going through \f$A^{\prime}\f$ and 
 *               \f$A^{\prime\prime}\f$ evaluated at \f$B\f$.
 * @param ec the elliptic curve to operate on
 * @param a a point in \f$E(F_p)\f$
 * @param aprime a point in \f$E(F_o)\f$
 * @param b a point in \f$E(F_p^2)\f$
 * @return SUCCESS if everything went right
 */
Status divisor_evaluateLine(Complex* result, const EllipticCurve ec, const AffinePoint a, const AffinePoint aprime,
                            const ComplexAffinePoint b);

#endif