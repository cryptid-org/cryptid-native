#ifndef __CRYPTID_ELLIPTICCURVE_AS_BINARY_H
#define __CRYPTID_ELLIPTICCURVE_AS_BINARY_H

#include "elliptic/EllipticCurve.h"

/**
 * ## Description
 *
 * A finite field on an elliptic curve of form \f$y^2 = x^3 + ax + b\f$
 * represented with binary values.
 */
typedef struct EllipticCurveAsBinary {
  /**
   * ## Description
   *
   * The binary representation of the coefficient a from the elliptic curve equation \f$y^2 = x^3 + ax + b\f$.
   */
  void *a;

  /**
   * ## Description
   *
   * The length of a.
   */
  size_t aLength;

  /**
   * ## Description
   *
   * The binary representation of the coefficient b from the elliptic curve equation \f$y^2 = x^3 + ax + b\f$.
   */
  void *b;

  /**
   * ## Description
   *
   * The length of b.
   */
  size_t bLength;

  /**
   * ## Description
   *
   * The binary representation of the order of the finite field.
   */
  void *fieldOrder;

  /**
   * ## Description
   *
   * The length of fieldOrder.
   */
  size_t fieldOrderLength;
} EllipticCurveAsBinary;

/**
 * ## Description
 *
 * Initializes a new EllipticCurve instance with the binary representation of arbitrary precision integers.
 *
 * ## Parameters
 *
 *   * ellipticCurveAsBinaryOutput
 *     * The EllipticCurveAsBinary to be initialized.
 *   * a
 *     * The binary representation of the coefficient \f$a\f$.
 *   * aLength
 *     * The length of the a coefficient's binary representation.
 *   * b
 *     * The binary representation of the coefficient \f$b\f$.
 *   * bLength
 *     * The length of the b coefficient's binary representation.
 *   * fieldOrder
 *     * The binary representation of the order of the finite field.
 *   * fieldOrderLength
 *     * The length of the finite field order's binary representation.
 */
void ellipticCurveAsBinary_init(
    EllipticCurveAsBinary *ellipticCurveAsBinaryOutput, const void *const a,
    const size_t aLength, const void *const b, const size_t bLength,
    const void *const fieldOrder, const size_t fieldOrderLength);

/**
 * ## Description
 *
 * Frees an EllipticCurveAsBinary.
 *
 * ## Parameters
 *
 *   * ellipticCurveAsBinary
 *     * The EllipticCurveAsBinary to be destroyed.
 */
void ellipticCurveAsBinary_destroy(EllipticCurveAsBinary ellipticCurveAsBinary);

/**
 * ## Description
 *
 * Converts an EllipticCurveAsBinary to EllipticCurve.
 *
 * ## Parameters
 *
 *   * ellipticCurveOutput
 *     * The EllipticCurve to be initialized based on an EllipticCurveAsBinary.
 *   * ellipticCurveAsBinary
 *     * The EllipticCurveAsBinary, which needs to be converted to EllipticCurve.
 */
void ellipticCurveAsBinary_toEllipticCurve(
    EllipticCurve *ellipticCurveOutput,
    const EllipticCurveAsBinary ellipticCurveAsBinary);

/**
 * ## Description
 *
 * Converts an EllipticCurve to EllipticCurveAsBinary.
 *
 * ## Parameters
 *
 *   * ellipticCurveAsBinaryOutput
 *     * The EllipticCurveAsBinary to be initialized based on an EllipticCurve.
 *   * ellipticCurve
 *     * The EllipticCurve, which needs to be converted to EllipticCurveAsBinary.
 */
void ellipticCurveAsBinary_fromEllipticCurve(
    EllipticCurveAsBinary *ellipticCurveAsBinaryOutput,
    const EllipticCurve ellipticCurve);

#endif