#ifndef __CRYPTID_COMPLEX_H
#define __CRYPTID_COMPLEX_H

#include <stddef.h>

#include "gmp.h"

#include "util/Status.h"
#include "util/Equality.h"

/**
 * ## Description
 *
 * Arbitrary precision complex number with integer-only real and imaginary
 * parts.
 */
typedef struct Complex {
  /**
   * ## Description
   *
   * The real part of the complex number.
   */
  mpz_t real;

  /**
   * ## Description
   *
   * The imaginary part of the complex number.
   */
  mpz_t imaginary;
} Complex;

/**
 * ## Description
 *
 * Initializes a new Complex number to (0, 0).
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The Complex number to be initialized.
 */
void complex_init(Complex *complexOutput);

/**
 * ## Description
 *
 * Initializes a new Complex with the specified arbitrary precision values.
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The Complex number to be initialized.
 *   * real
 *     * The real part.
 *   * imaginary
 *     * The imaginary part.
 */
void complex_initMpz(Complex *complexOutput, const mpz_t real,
                     const mpz_t imaginary);

/**
 * ## Description
 *
 * Initializes a new Complex with the specified long values.
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The Complex number to be initialized.
 *   * real
 *     * The real part.
 *   * imaginary
 *     * The imaginary part.
 */
void complex_initLong(Complex *complexOutput, const long real,
                      const long imaginary);

/**
 * ## Description
 *
 * Initializes a new Complex with an arbitrary precision real part and a long
 * imaginary part.
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The Complex number to be initialized.
 *   * real
 *     * The real part.
 *   * imaginary
 *     * The imaginary part.
 */
void complex_initMpzLong(Complex *complexOutput, const mpz_t real,
                         const long imaginary);

/**
 * ## Description
 *
 * Initializes a new Complex with a long real part and ann arbitrary precision
 * imaginary part.
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The Complex number to be initialized.
 *   * real
 *     * The real part.
 *   * imaginary
 *     * The imaginary part.
 */
void complex_initLongMpz(Complex *complexOutput, const long real,
                         const mpz_t imaginary);

/**
 * ## Description
 *
 * Checks if two Complex instances hold the same values.
 *
 * ## Parameters
 *
 *   * complex1
 *     * A Complex to check.
 *   * complex2
 *     * A Complex to check.
 *
 * ## Return Value
 *
 * 1 if the two Complex instances are equal, 0 otherwise.
 */
CryptidEqualityResult complex_isEquals(const Complex complex1, const Complex complex2);

/**
 * ## Description
 *
 * Frees a Complex instance. After calling this function on a Complex instance,
 * that instance should not be used anymore.
 *
 * ## Parameters
 *
 *   * complex
 *     * The Complex to be destroyed.
 */
void complex_destroy(const Complex complex);

/**
 * ## Description
 *
 * Frees multiple Complex instances. After calling this function on a Complex
 * instance, that instance should not be used anymore.
 *
 * ## Parameters
 *
 *   * argumentCount
 *     * The count of instances to be destroyed.
 */
void complex_destroyMany(const size_t argumentCount, ...);

/**
 * ## Description
 *
 * Adds two Complex valus with respect to the specified modulus.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the addition.
 *   * complex1
 *     * A value to add.
 *   * complex2
 *     * A value to add.
 *   * p
 *     * The modulus.
 */
void complex_modAdd(Complex *result, const Complex complex1,
                    const Complex complex2, const mpz_t p);

/**
 * ## Description
 *
 * Calculates the additive inverse of the specified Complex with respect to the
 * specified modulus.
 *
 * ## Parameters
 *
 *   * result
 *     * The additive inverse.
 *   * complex
 *     * The Complex to invert.
 *   * p
 *     * The modulus.
 */
void complex_additiveInverse(Complex *result, const Complex complex,
                             const mpz_t p);

/**
 * ## Description
 *
 * Adds a Complex and an integer with respect to the specified modulus.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the addition.
 *   * complex
 *     * A Complex.
 *   * s
 *     * A scalar.
 *   * p
 *     * The modulus.
 */
void complex_modAddScalar(Complex *result, const Complex complex, const mpz_t s,
                          const mpz_t p);

/**
 * ## Description
 *
 * Multiplies two Complex values with respect to the specified modulus.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the multiplication.
 *   * complex1
 *     * A Complex to multiply.
 *   * complex2
 *     * A Complex to multiply.
 *   * p
 *     * The modulus.
 */
void complex_modMul(Complex *result, const Complex complex1,
                    const Complex complex2, const mpz_t p);

/**
 * ## Description
 *
 * Raised a Complex value to the specified exponent modulo p.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the exponentiation.
 *   * complex
 *     * The base.
 *   * exp
 *     * The exponent.
 *   * p
 *     * The modulus.
 */
void complex_modPow(Complex *result, const Complex complex, const mpz_t exp,
                    const mpz_t p);

/**
 * ## Description
 *
 * Multiplies a Complex value with an integer.
 *
 * ## Parameters
 *
 *   * result
 *     * The result of the multiplication.
 *   * complex
 *     * A Complex.
 *   * s
 *     * The scalar.
 *   * p
 *     * The modulus.
 */
void complex_modMulScalar(Complex *result, const Complex complex, const mpz_t s,
                          const mpz_t p);

/**
 * ## Description
 *
 * Calculates the multiplicate inverse of a Complex with respect to p.
 *
 * ## Parameters
 *
 *   * result
 *     * Out parameter to the multiplicative inverse. On CRYPTID_SUCCESS, this
 * should be destroyed by the caller.
 *   * complex
 *     * A Complex.
 *   * p
 *     * The modulus.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if complex has a multiplicative inverse, HAS_NO_MUL_INV error
 * otherwise.
 */
CryptidStatus complex_multiplicativeInverse(Complex *result,
                                            const Complex complex,
                                            const mpz_t p);

#endif