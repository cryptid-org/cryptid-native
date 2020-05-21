#ifndef __CRYPTID_ELLIPTICCURVE_H
#define __CRYPTID_ELLIPTICCURVE_H

#include "gmp.h"
#include "util/Validation.h"

/**
 * ## Description
 *
 * A finite field on an elliptic curve of form \f$y^2 = x^3 + ax + b\f$.
 */
typedef struct EllipticCurve {
  /**
   * ## Description
   *
   * The coefficient a from the elliptic curve equation \f$y^2 = x^3 + ax + b\f$.
   */
  mpz_t a;

  /**
   * ## Description
   *
   * The coefficient b from the elliptic curve equation \f$y^2 = x^3 + ax + b\f$.
   */
  mpz_t b;

  /**
   * ## Description
   *
   * The order of the finite field.
   */
  mpz_t fieldOrder;
} EllipticCurve;

/**
 * ## Description
 *
 * Initializes a new EllipticCurve instance with arbitrary precision integers.
 *
 * ## Parameters
 *
 *   * ellipticCurveOutput
 *     * The EllipticCurve to be initialized.
 *   * a
 *     * Coefficient \f$a\f$.
 *   * b
 *     * Coefficient \f$b\f$.
 *   * fieldOrder
 *     * The order of the finite field.
 */
void ellipticCurve_init(EllipticCurve *ellipticCurveOutput, const mpz_t a,
                        const mpz_t b, const mpz_t fieldOrder);

/**
 * ## Description
 *
 * Initializes a new EllipticCurve instance with longs.
 *
 * ## Parameters
 *
 *   * ellipticCurveOutput
 *     * The EllipticCurve to be initialized.
 *   * a
 *     * Coefficient \f$a\f$.
 *   * b
 *     * Coefficient \f$b\f$.
 *   * fieldOrder
 *     * The order of the finite field.
 */
void ellipticCurve_initLong(EllipticCurve *ellipticCurveOutput, const long a,
                            const long b, const long fieldOrder);

/**
 * ## Description
 *
 * Frees an EllipticCurve.
 *
 * ## Parameters
 *
 *   * ellipticCurve
 *     * The EllipticCurve to be destroyed.
 */
void ellipticCurve_destroy(EllipticCurve ellipticCurve);

/**
 * ## Description
 *
 * Validates that the specified EllipticCurve instance is a Type-1 elliptic curve.
 *
 * ## Parameters
 *
 *   * ellipticCurve
 *     * The struct to check.
 *
 * ## Return Value
 *
 * CRYPTID_VALIDATION_SUCCESS if the struct is valid.
 */
CryptidValidationResult
ellipticCurve_isTypeOne(const EllipticCurve ellipticCurve);

#endif
