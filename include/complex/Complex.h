/**
 * TODO
 */
#ifndef __CRYPTID_COMPLEX_H
#define __CRYPTID_COMPLEX_H

#include <stddef.h>

#include "gmp.h"

#include "util/Equality.h"
#include "util/Status.h"

/**
 * ## Description
 *
 * Arbitrary precision complex number ($`a + bi`$) with integer-only real and
 * imaginary parts.
 *
 * ## See Also
 *
 *   * [ComplexAsBinary](codebase://complex/ComplexAsBinary.h#ComplexAsBinary)
 */
typedef struct Complex {
  /**
   * The real part $`a`$ of the complex number $`a + bi`$.
   */
  mpz_t real;

  /**
   * The imaginary part $`b`$ of the complex number $`a + bi`$.
   */
  mpz_t imaginary;
} Complex;

/**
 * ## Description
 *
 * Initializes a new complex number to $`0 + 0i`$.
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The pointer to the complex number to be initialized. No memory
 * allocation required before the function call. Should be destroyed by the
 * caller if not needed anymore.
 *
 * ## See Also
 *
 *   * [complex_initMpz](codebase://complex/Complex.h#complex_initMpz)
 *   * [complex_initLong](codebase://complex/Complex.h#complex_initLong)
 *   * [complex_initMpzLong](codebase://complex/Complex.h#complex_initMpzLong)
 *   * [complex_initLongMpz](codebase://complex/Complex.h#complex_initLongMpz)
 *   * [complex_destroy](codebase://complex/Complex.h#complex_destroy)
 *   * [complex_destroyMany](codebase://complex/Complex.h#complex_destroyMany)
 */
void complex_init(Complex *complexOutput);

/**
 * ## Description
 *
 * Initializes a new complex number and sets it's value to the specified
 * arbitrary precision integers ($`real + imaginary \cdot i`$).
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The pointer to the complex number to be initialized. No memory
 * allocation required before the function call. Should be destroyed by the
 * caller if not needed anymore.
 *   * real
 *     * The real part $`a`$ of the complex number $`a + bi`$.
 *   * imaginary
 *     * The imaginary part $`b`$ of the complex number $`a + bi`$.
 *
 * ## See Also
 *
 *   * [complex_init](codebase://complex/Complex.h#complex_init)
 *   * [complex_initLong](codebase://complex/Complex.h#complex_initLong)
 *   * [complex_initMpzLong](codebase://complex/Complex.h#complex_initMpzLong)
 *   * [complex_initLongMpz](codebase://complex/Complex.h#complex_initLongMpz)
 *   * [complex_destroy](codebase://complex/Complex.h#complex_destroy)
 *   * [complex_destroyMany](codebase://complex/Complex.h#complex_destroyMany)
 */
void complex_initMpz(Complex *complexOutput, const mpz_t real,
                     const mpz_t imaginary);

/**
 * ## Description
 *
 * Initializes a new complex number and sets it's value to the specified `long`
 * values ($`real + imaginary \cdot i`$).
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The pointer to the complex number to be initialized. No memory
 * allocation required before the function call. Should be destroyed by the
 * caller if not needed anymore.
 *   * real
 *     * The real part $`a`$ of the complex number $`a + bi`$.
 *   * imaginary
 *     * The imaginary part $`b`$ of the complex number $`a + bi`$.
 *
 * ## See Also
 *
 *   * [complex_init](codebase://complex/Complex.h#complex_init)
 *   * [complex_initMpz](codebase://complex/Complex.h#complex_initMpz)
 *   * [complex_initMpzLong](codebase://complex/Complex.h#complex_initMpzLong)
 *   * [complex_initLongMpz](codebase://complex/Complex.h#complex_initLongMpz)
 *   * [complex_destroy](codebase://complex/Complex.h#complex_destroy)
 *   * [complex_destroyMany](codebase://complex/Complex.h#complex_destroyMany)
 */
void complex_initLong(Complex *complexOutput, const long real,
                      const long imaginary);

/**
 * ## Description
 *
 * Initializes a new complex number and sets it's value to the specified
 * arbitrary precision real part and `long` imaginary part ($`real + imaginary
 * \cdot i`$).
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The pointer to the complex number to be initialized. No memory
 * allocation required before the function call. Should be destroyed by the
 * caller if not needed anymore.
 *   * real
 *     * The real part $`a`$ of the complex number $`a + bi`$.
 *   * imaginary
 *     * The imaginary part $`b`$ of the complex number $`a + bi`$.
 *
 * ## See Also
 *
 *   * [complex_init](codebase://complex/Complex.h#complex_init)
 *   * [complex_initMpz](codebase://complex/Complex.h#complex_initMpz)
 *   * [complex_initLong](codebase://complex/Complex.h#complex_initLong)
 *   * [complex_initLongMpz](codebase://complex/Complex.h#complex_initLongMpz)
 *   * [complex_destroy](codebase://complex/Complex.h#complex_destroy)
 *   * [complex_destroyMany](codebase://complex/Complex.h#complex_destroyMany)
 */
void complex_initMpzLong(Complex *complexOutput, const mpz_t real,
                         const long imaginary);

/**
 * ## Description
 *
 * Initializes a new complex number and sets it's value to the specified `long`
 * real part and arbitrary precision imaginary part ($`real + imaginary \cdot
 * i`$).
 *
 * ## Parameters
 *
 *   * complexOutput
 *     * The pointer to the complex number to be initialized. No memory
 * allocation required before the function call. Should be destroyed by the
 * caller if not needed anymore.
 *   * real
 *     * The real part $`a`$ of the complex number $`a + bi`$.
 *   * imaginary
 *     * The imaginary part $`b`$ of the complex number $`a + bi`$.
 *
 * ## See Also
 *
 *   * [complex_init](codebase://complex/Complex.h#complex_init)
 *   * [complex_initMpz](codebase://complex/Complex.h#complex_initMpz)
 *   * [complex_initLong](codebase://complex/Complex.h#complex_initLong)
 *   * [complex_initMpzLong](codebase://complex/Complex.h#complex_initMpzLong)
 *   * [complex_destroy](codebase://complex/Complex.h#complex_destroy)
 *   * [complex_destroyMany](codebase://complex/Complex.h#complex_destroyMany)
 */
void complex_initLongMpz(Complex *complexOutput, const long real,
                         const mpz_t imaginary);

/**
 * ## Description
 *
 * Checks if two complex numbers hold the same values.
 *
 * ## Parameters
 *
 *   * complex1
 *     * The first of the two complex numbers to be checked for equality.
 *   * complex2
 *     * The second of the two complex numbers to be checked for equality.
 *
 * ## Return Value
 *
 *   * CRYPTID_EQUAL
 *     * If the two given complex numbers hold the same values.
 *   * CRYPTID_UNEQUAL
 *     * If the two given complex numbers do not hold the same values.
 */
CryptidEqualityResult complex_isEquals(const Complex complex1,
                                       const Complex complex2);

/**
 * ## Description
 *
 * Frees a [Complex](codebase://complex/Complex.h#Complex) instance. After
 * calling this function on the instance, that instance should not be used
 * before reinitialization.
 *
 * ## Parameters
 *
 *   * complex
 *     * The instance to be destroyed.
 *
 * ## See Also
 *
 *   * [complex_destroyMany](codebase://complex/Complex.h#complex_destroyMany)
 *   * [complex_init](codebase://complex/Complex.h#complex_init)
 *   * [complex_initMpz](codebase://complex/Complex.h#complex_initMpz)
 *   * [complex_initLong](codebase://complex/Complex.h#complex_initLong)
 *   * [complex_initMpzLong](codebase://complex/Complex.h#complex_initMpzLong)
 *   * [complex_initLongMpz](codebase://complex/Complex.h#complex_initLongMpz)
 */
void complex_destroy(const Complex complex);

/**
 * ## Description
 *
 * Frees multiple [Complex](codebase://complex/Complex.h#Complex) instances.
 * After calling this function on the instances, those should not be used before
 * reinitialization.
 *
 * ## Parameters
 *
 *   * argumentCount
 *     * The count of instances to be destroyed.
 *
 * ## See Also
 *
 *   * [complex_destroy](codebase://complex/Complex.h#complex_destroy)
 *   * [complex_init](codebase://complex/Complex.h#complex_init)
 *   * [complex_initMpz](codebase://complex/Complex.h#complex_initMpz)
 *   * [complex_initLong](codebase://complex/Complex.h#complex_initLong)
 *   * [complex_initMpzLong](codebase://complex/Complex.h#complex_initMpzLong)
 *   * [complex_initLongMpz](codebase://complex/Complex.h#complex_initLongMpz)
 */
void complex_destroyMany(const size_t argumentCount, ...);

/**
 * ## Description
 *
 * Adds two complex numbers with respect to the specified modulus. Calculated as
 * $`(r_1 + r_2 \mod m, i_1 + i_2 \mod m)`$.
 *
 * ## Parameters
 *
 *   * sum
 *     * Out parameter to the addition. No memory allocation required before the
 * function call. Should be destroyed by the caller if not needed anymore.
 *   * augend
 *     * The complex number to which the addend is added.
 *   * addend
 *     * The complex number that is added to the augend.
 *   * modulus
 *     * The modulus of the operation.
 *
 * ## See Also
 *
 *   * [complex_modAddInteger](codebase://complex/Complex.h#complex_modAddInteger)
 */
void complex_modAdd(Complex *sum, const Complex augend, const Complex addend,
                    const mpz_t modulus);

/**
 * ## Description
 *
 * Calculates the additive inverse of the specified complex number with respect
 * to the specified modulus. Calculated as $`(-r \mod m, -i \mod m)`$.
 *
 * ## Parameters
 *
 *   * inverse
 *     * Out parameter to the additive inverse. No memory allocation required
 * before the function call. Should be destroyed by the caller if not needed
 * anymore.
 *   * operand
 *     * The complex number to invert.
 *   * modulus
 *     * The modulus of the operation.
 *
 * ## See Also
 *
 *   * [complex_multiplicativeInverse](codebase://complex/Complex.h#complex_multiplicativeInverse)
 */
void complex_additiveInverse(Complex *inverse, const Complex operand,
                             const mpz_t modulus);

/**
 * ## Description
 *
 * Adds a complex number and an arbitrary precision integer with respect to the
 * specified modulus. Calculated as $`(r + a \mod m, i)`$.
 *
 * ## Parameters
 *
 *   * sum
 *     * Out parameter to the addition. No memory allocation required before the
 * function call. Should be destroyed by the caller if not needed anymore.
 *   * augend
 *     * The complex number to which the addend is added.
 *   * addend
 *     * The integer number that is added to the augend.
 *   * modulus
 *     * The modulus of the operation.
 *
 * ## See Also
 *
 *   * [complex_modAdd](codebase://complex/Complex.h#complex_modAdd)
 */
void complex_modAddInteger(Complex *sum, const Complex augend,
                           const mpz_t addend, const mpz_t modulus);

/**
 * ## Description
 *
 * Multiplies two complex numbers with respect to the specified modulus.
 * Calculated as $`((r_1 \cdot r_2 - i_1 \cdot i_2) \mod m, (i_1 \cdot r_2 + r_1
 * \cdot i_2) \mod m)`$.
 *
 * ## Parameters
 *
 *   * product
 *     * Out parameter to the multiplication. No memory allocation required
 * before the function call. Should be destroyed by the caller if not needed
 * anymore.
 *   * multiplier
 *     * The complex number to multiply with.
 *   * multiplicand
 *     * The complex number to be multiplied by the multiplier.
 *   * modulus
 *     * The modulus of the operation.
 *
 * ## See Also
 *
 *   * [complex_modMulInteger](codebase://complex/Complex.h#complex_modMulInteger)
 */
void complex_modMul(Complex *product, const Complex multiplier,
                    const Complex multiplicand, const mpz_t modulus);

/**
 * ## Description
 *
 * Raises a complex number to the specified power with respect to the specified
 * modulus. Calculated with the square-and-multiply method.
 *
 * ## Parameters
 *
 *   * power
 *     * Out parameter to the exponentiation. No memory allocation required
 * before the function call. Should be destroyed by the caller if not needed
 * anymore.
 *   * base
 *     * The base of the exponentiation.
 *   * exponent
 *     * The exponent of the exponentiation.
 *   * modulus
 *     * The modulus of the operation.
 */
void complex_modPow(Complex *power, const Complex base, const mpz_t exponent,
                    const mpz_t modulus);

/**
 * ## Description
 *
 * Multiplies a complex number with an arbitrary precision integer with respect
 to the specified modulus. Calculated as $`(n \cdot r \mod m, n \cdot i \mod
 m)`$.
 *
 * ## Parameters
 *
 *   * product
 *     * Out parameter to the multiplication. No memory allocation required
 before the function call. Should be destroyed by the caller if not needed
 anymore.
 *   * multiplier
 *     * The integer number to multiply with.
 *   * multiplicand
 *     * The complex number to be multiplied by the multiplier.
 *   * modulus
 *     * The modulus of the operation.
 *
 * ## See Also
 *
 *   * [complex_modMul](codebase://complex/Complex.h#complex_modMul)
 */
void complex_modMulInteger(Complex *product, const mpz_t multiplier,
                           const Complex multiplicand, const mpz_t modulus);

/**
 * ## Description
 *
 * Calculates the multiplicate inverse of a complex number with respect to the
 * modulus. The inverse of $`c`$ is $`c^{-1} = \frac{1}{c}`$. Calculated as
 * $`(frac{r}{r^2+i^2}, -\frac{i}{r^2+i^2})`$.
 *
 * ## Parameters
 *
 *   * inverse
 *     * Out parameter to the multiplicative inverse. No memory allocation
 * required before the function call. On CRYPTID_SUCCESS, this should be
 * destroyed by the caller.
 *   * operand
 *     * The complex number to invert.
 *   * modulus
 *     * The modulus of the operation.
 *
 * ## Return Value
 *
 *   * CRYPTID_SUCCESS
 *     * If the multiplicative inverse was calculated correctly.
 *   * CRYPTID_HAS_NO_MUL_INV_ERROR
 *     * If there is no multiplicative inverse of the given value.
 *
 * ## See Also
 *
 *   * [complex_modAddInteger](codebase://complex/Complex.h#complex_modAddInteger)
 */
CryptidStatus complex_multiplicativeInverse(Complex *inverse,
                                            const Complex operand,
                                            const mpz_t modulus);

#endif