#ifndef __CRYPTID_TATEPAIRING_H
#define __CRYPTID_TATEPAIRING_H

#include "gmp.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Computes the Tate pairing over Type-1 elliptic curves.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter to the resulting Complex value. On CRYPTID_SUCCESS, this should be destroyed by the caller.
 *   * p
 *     * A point of \f$E[r]\f$.
 *   * b
 *     * A point of \f$E[r]\f$.
 *   * embeddingDegree
 *     * The embedding degree of the curve.
 *   * subgroupOrder
 *     * The order of the subgroup.
 *   * ellipticCurve
 *     * The elliptic curve to operate on.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if everything went right.
 */
CryptidStatus tate_performPairing(Complex *result, const AffinePoint p,
                                  const AffinePoint b,
                                  const int embeddingDegree,
                                  const mpz_t subgroupOrder,
                                  const EllipticCurve ellipticCurve);

#endif
