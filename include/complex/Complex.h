#ifndef __CRYPTID_COMPLEX_H
#define __CRYPTID_COMPLEX_H

#include <stddef.h>

#include "gmp.h"

#include "util/Equality.h"
#include "util/Status.h"

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
 * CRYPTID_EQUAL if the two Complex instances are equal, CRYPTID_UNEQUAL
 * otherwise.
 */
CryptidEqualityResult complex_isEquals(const Complex complex1,
                                       const Complex complex2);

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
 *   * sum
 *     * The result of the addition.
 *   * augend
 *     * The complex number to which the addend is added.
 *   * addend
 *     * The complex number that is added to the augend.
 *   * modulus
 *     * The modulus.
 */
void complex_modAdd(Complex *sum, const Complex augend, const Complex addend,
                    const mpz_t modulus);

/**
 * ## Description
 *
 * Calculates the additive inverse of the specified Complex with respect to the
 * specified modulus.
 *
 * ## Parameters
 *
 *   * inverse
 *     * The additive inverse.
 *   * operand
 *     * The Complex to invert.
 *   * modulus
 *     * The modulus.
 */
void complex_additiveInverse(Complex *inverse, const Complex operand,
                             const mpz_t modulus);

/**
 * ## Description
 *
 * Adds a Complex and an integer with respect to the specified modulus.
 *
 * ## Parameters
 *
 *   * sum
 *     * The result of the addition.
 *   * augend
 *     * The complex number to which the addend is added.
 *   * addend
 *     * The integer number that is added to the augend.
 *   * modulus
 *     * The modulus.
 */
void complex_modAddInteger(Complex *sum, const Complex augend,
                           const mpz_t addend, const mpz_t modulus);

/**
 * ## Description
 *
 * Multiplies two Complex values with respect to the specified modulus.
 *
 * ## Parameters
 *
 *   * product
 *     * The result of the multiplication.
 *   * multiplier
 *     * The complex number to multiply with.
 *   * multiplicand
 *     * The complex number to be multiplied by the multiplier.
 *   * modulus
 *     * The modulus.
 */
void complex_modMul(Complex *product, const Complex multiplier,
                    const Complex multiplicand, const mpz_t modulus);

/**
 * ## Description
 *
 * Raised a Complex value to the specified exponent modulo p.
 *
 * ## Parameters
 *
 *   * power
 *     * The result of the exponentiation.
 *   * base
 *     * The base of the exponentiation.
 *   * exponent
 *     * The exponent of the exponentiation.
 *   * modulus
 *     * The modulus.
 */
void complex_modPow(Complex *power, const Complex base, const mpz_t exponent,
                    const mpz_t modulus);

/**
 * ## Description
 *
 * Multiplies a Complex value with an integer.
 *
 * ## Parameters
 *
 *   * product
 *     * The result of the multiplication.
 *   * multiplier
 *     * The integer number to multiply with.
 *   * multiplicand
 *     * The complex number to be multiplied by the multiplier.
 *   * modulus
 *     * The modulus.
 */
void complex_modMulInteger(Complex *product, const mpz_t multiplier,
                           const Complex multiplicand, const mpz_t modulus);

/**
 * ## Description
 *
 * Calculates the multiplicate inverse of a Complex with respect to the modulus.
 *
 * ## Parameters
 *
 *   * inverse
 *     * Out parameter to the multiplicative inverse. On CRYPTID_SUCCESS, this
 * should be destroyed by the caller.
 *   * operand
 *     * The Complex to invert.
 *   * modulus
 *     * The modulus.
 *
 * ## Return Value
 *
 * CRYPTID_SUCCESS if complex has a multiplicative inverse, HAS_NO_MUL_INV error
 * otherwise.
 */
CryptidStatus complex_multiplicativeInverse(Complex *inverse,
                                            const Complex operand,
                                            const mpz_t modulus);

#endif