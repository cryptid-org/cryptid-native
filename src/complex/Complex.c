#include <stdarg.h>

#include "complex/Complex.h"

void complex_init(Complex *complexOutput) {
  mpz_inits(complexOutput->real, complexOutput->imaginary, NULL);
}

void complex_initMpz(Complex *complexOutput, const mpz_t real,
                     const mpz_t imaginary) {
  complex_init(complexOutput);

  mpz_set(complexOutput->real, real);
  mpz_set(complexOutput->imaginary, imaginary);
}

void complex_initLong(Complex *complexOutput, const long real,
                      const long imaginary) {
  complex_init(complexOutput);

  mpz_set_si(complexOutput->real, real);
  mpz_set_si(complexOutput->imaginary, imaginary);
}

void complex_initMpzLong(Complex *complexOutput, const mpz_t real,
                         const long imaginary) {
  complex_init(complexOutput);

  mpz_set(complexOutput->real, real);
  mpz_set_si(complexOutput->imaginary, imaginary);
}

void complex_initLongMpz(Complex *complexOutput, const long real,
                         const mpz_t imaginary) {
  complex_init(complexOutput);

  mpz_set_si(complexOutput->real, real);
  mpz_set(complexOutput->imaginary, imaginary);
}

CryptidEqualityResult complex_isEquals(const Complex complex1, const Complex complex2) {
  if (!mpz_cmp(complex1.real, complex2.real) &&
      !mpz_cmp(complex1.imaginary, complex2.imaginary)) {
    return CRYPTID_EQUAL;
  }

  return CRYPTID_UNEQUAL;
}

void complex_destroy(Complex complex) {
  mpz_clears(complex.real, complex.imaginary, NULL);
}

void complex_destroyMany(const size_t argumentCount, ...) {
  va_list args;
  va_start(args, argumentCount);

  for (size_t i = 0; i < argumentCount; ++i) {
    Complex complex = (va_arg(args, Complex));
    complex_destroy(complex);
  }

  va_end(args);
}

void complex_modAdd(Complex *sum, const Complex augend,
                    const Complex addend, const mpz_t modulus) {
  // Calculated as
  // \f$(r_1 + r_2 \mod m, i_1 + i_2 \mod m)\f$.
  mpz_t sumReal, sumImaginary;
  mpz_inits(sumReal, sumImaginary, NULL);

  mpz_add(sumReal, augend.real, addend.real);
  mpz_mod(sumReal, sumReal, modulus);

  mpz_add(sumImaginary, augend.imaginary, addend.imaginary);
  mpz_mod(sumImaginary, sumImaginary, modulus);

  complex_initMpz(sum, sumReal, newImaginary);
  mpz_clears(sumReal, sumImaginary, NULL);
}

void complex_additiveInverse(Complex *inverse, const Complex operand,
                             const mpz_t modulus) {
  // Calculated as
  // \f$(-r \mod m, -i \mod m)\f$.
  mpz_t inverseReal, inverseImaginary;
  mpz_inits(inverseReal, inverseImaginary, NULL);

  mpz_neg(inverseReal, operand.real);
  mpz_mod(inverseReal, inverseReal, modulus);

  mpz_neg(inverseImaginary, operand.imaginary);
  mpz_mod(inverseImaginary, inverseImaginary, modulus);

  complex_initMpz(inverse, inverseReal, inverseImaginary);
  mpz_clears(inverseReal, inverseImaginary, NULL);
}

void complex_modAddInteger(Complex *sum, const Complex augend, const mpz_t addend,
                          const mpz_t modulus) {
  // Calculated as
  // \f$(r + a \mod m, i)\f$.
  mpz_t sumReal;
  mpz_init(sumReal);

  mpz_add(sumReal, augend.real, addend);
  mpz_mod(sumReal, sumReal, modulus);

  complex_initMpz(sum, sumReal, augend.imaginary);
  mpz_clear(sumReal);
}

void complex_modMul(Complex *product, const Complex multiplier,
                    const Complex multiplicand, const mpz_t modulus) {
  // Calculated as
  // \f$((r_1 \cdot r_2 - i_1 \cdot i_2) \mod m, (i_1 \cdot r_2 + r_1 \cdot i_2)
  // \mod m)\f$.
  mpz_t productReal, productImaginary, leftProduct, rightProduct;
  mpz_inits(productReal, productImaginary, leftProduct, rightProduct, NULL);

  mpz_mul(leftProduct, multiplier.real, multiplicand.real);
  mpz_mul(rightProduct, multiplier.imaginary, multiplicand.imaginary);
  mpz_sub(productReal, leftProduct, rightProduct);
  mpz_mod(productReal, productReal, modulus);

  mpz_mul(leftProduct, multiplier.imaginary, multiplicand.real);
  mpz_mul(rightProduct, multiplier.real, multiplicand.imaginary);
  mpz_add(productImaginary, leftProduct, rightProduct);
  mpz_mod(productImaginary, productImaginary, modulus);

  complex_initMpz(product, productReal, productImaginary);
  mpz_clears(productReal, productImaginary, leftProduct, rightProduct, NULL);
}

void complex_modPow(Complex *power, const Complex base, const mpz_t exponent,
                    const mpz_t modulus) {
  if (!mpz_cmp_ui(modulus, 1)) {
    complex_initLong(power, 0, 0);
    return;
  }

  mpz_t baseRealCopy, baseImaginaryCopy, exponentCopy, exponentRemainder;
  mpz_inits(baseRealCopy, baseImaginaryCopy, exponentCopy, exponentRemainder, NULL);

  complex_initLong(power, 1, 0);

  mpz_set(baseRealCopy, base.real);
  mpz_mod(baseRealCopy, baseRealCopy, modulus);

  mpz_set(baseImaginaryCopy, base.imaginary);
  mpz_mod(baseImaginaryCopy, baseImaginaryCopy, modulus);

  Complex baseCopy;
  complex_initMpz(&baseCopy, baseRealCopy, baseImaginaryCopy);

  mpz_set(exponentCopy, exponent);

  while (mpz_cmp_ui(exponentCopy, 0) > 0) {
    mpz_mod_ui(exponentRemainder, exponentCopy, 2);
    if (!mpz_cmp_ui(exponentRemainder, 1)) {
      Complex tmp;
      complex_modMul(&tmp, baseCopy, *power, modulus);
      complex_destroy(*power);
      *power = tmp;
    }

    mpz_fdiv_q_2exp(exponentCopy, exponentCopy, 1);

    Complex tmp;
    complex_modMul(&tmp, baseCopy, baseCopy, modulus);
    complex_destroy(baseCopy);
    baseCopy = tmp;
  }

  complex_destroy(baseCopy);
  mpz_clears(baseRealCopy, baseImaginaryCopy, exponentCopy, exponentRemainder, NULL);
}

void complex_modMulInteger(Complex *product, const mpz_t multiplier, const Complex multiplicand,
                          const mpz_t modulus) {
  // Calculated as
  // \f$(r \cdot s \mod m, i \cdot s \mod m)\f$.
  mpz_t productReal, productImaginary;
  mpz_inits(productReal, productImaginary, NULL);

  mpz_mul(productReal, multiplier, multiplicand.real);
  mpz_mod(productReal, productReal, modulus);

  mpz_mul(productImaginary, multiplier, multiplicand.imaginary);
  mpz_mod(productImaginary, productImaginary, modulus);

  complex_initMpz(product, productReal, productImaginary);
  mpz_clears(productReal, productImaginary, NULL);
}

// The inverse of z is z^{-1} = \frac{1}{z} =
// \frac{r}{r^2+i^2}-\frac{i}{r^2+i^2}
CryptidStatus complex_multiplicativeInverse(Complex *inverse,
                                            const Complex operand,
                                            const mpz_t modulus) {
  mpz_t inverseReal, inverseImaginary, denominator, opRealSquare, opImagSquare, denomInverse, negImaginary;

  // (0, 0) has no multiplicative inverse.
  if (!mpz_cmp_ui(operand.real, 0) && !mpz_cmp_ui(operand.imaginary, 0)) {
    return CRYPTID_HAS_NO_MUL_INV_ERROR;
  }

  mpz_inits(inverseReal, inverseImaginary, denominator, opRealSquare, opImagSquare, denomInverse, negImaginary,
            NULL);

  // If the Complex instance only holds a real value, we can fallback to
  // simple inverse: \f$(r^{-1}, 0)\f$.
  if (!mpz_cmp_ui(operand.imaginary, 0)) {
    mpz_invert(inverseReal, operand.real, modulus);
    complex_initMpzLong(inverse, inverseReal, 0);

    mpz_clears(inverseReal, inverseImaginary, denominator, opRealSquare, opImagSquare, denomInverse,
               negImaginary, NULL);
    return CRYPTID_SUCCESS;
  }

  // Likewise, if the Complex instance only holds an imaginary value, we
  // can simplify things: \f$(0, -i^{-1})\f$.
  if (!mpz_cmp_ui(operand.real, 0)) {
    mpz_invert(inverseImaginary, operand.imaginary, modulus);
    mpz_neg(inverseImaginary, inverseImaginary);
    mpz_mod(inverseImaginary, inverseImaginary, modulus);
    complex_initLongMpz(inverse, 0, inverseImaginary);

    mpz_clears(inverseReal, inverseImaginary, denominator, opRealSquare, opImagSquare, denomInverse,
               negImaginary, NULL);
    return CRYPTID_SUCCESS;
  }

  mpz_pow_ui(opRealSquare, operand.real, 2);
  mpz_pow_ui(opImagSquare, operand.imaginary, 2);
  mpz_add(denominator, opRealSquare, opImagSquare);
  mpz_mod(denominator, denominator, modulus);

  mpz_invert(denomInverse, denominator, modulus);

  mpz_mul(inverseReal, operand.real, denomInverse);
  mpz_mod(inverseReal, inverseReal, modulus);

  mpz_neg(negImaginary, operand.imaginary);
  mpz_mod(negImaginary, negImaginary, modulus);

  mpz_mul(inverseImaginary, negImaginary, denomInverse);
  mpz_mod(inverseImaginary, inverseImaginary, modulus);

  complex_initMpz(inverse, inverseReal, inverseImaginary);
  mpz_clears(inverseReal, inverseImaginary, denominator, opRealSquare, opImagSquare, denomInverse, negImaginary,
             NULL);
  return CRYPTID_SUCCESS;
}
