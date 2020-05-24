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
  mpz_t newReal, newImaginary;
  mpz_inits(newReal, newImaginary, NULL);

  mpz_add(newReal, augend.real, addend.real);
  mpz_mod(newReal, newReal, modulus);

  mpz_add(newImaginary, augend.imaginary, addend.imaginary);
  mpz_mod(newImaginary, newImaginary, modulus);

  complex_initMpz(sum, newReal, newImaginary);
  mpz_clears(newReal, newImaginary, NULL);
}

void complex_additiveInverse(Complex *inverse, const Complex operand,
                             const mpz_t modulus) {
  // Calculated as
  // \f$(-r \mod m, -i \mod m)\f$.
  mpz_t newReal, newImaginary;
  mpz_inits(newReal, newImaginary, NULL);

  mpz_neg(newReal, operand.real);
  mpz_mod(newReal, newReal, modulus);

  mpz_neg(newImaginary, operand.imaginary);
  mpz_mod(newImaginary, newImaginary, modulus);

  complex_initMpz(inverse, newReal, newImaginary);
  mpz_clears(newReal, newImaginary, NULL);
}

void complex_modAddInteger(Complex *sum, const Complex augend, const mpz_t addend,
                          const mpz_t modulus) {
  // Calculated as
  // \f$(r + a \mod m, i)\f$.
  mpz_t newReal;
  mpz_init(newReal);

  mpz_add(newReal, augend.real, addend);
  mpz_mod(newReal, newReal, modulus);

  complex_initMpz(sum, newReal, augend.imaginary);
  mpz_clear(newReal);
}

void complex_modMul(Complex *product, const Complex multiplier,
                    const Complex multiplicand, const mpz_t modulus) {
  // Calculated as
  // \f$((r_1 \cdot r_2 - i_1 \cdot i_2) \mod m, (i_1 \cdot r_2 + r_1 \cdot i_2)
  // \mod m)\f$.
  mpz_t r, i, leftPart, rightPart;
  mpz_inits(r, i, leftPart, rightPart, NULL);

  mpz_mul(leftPart, multiplier.real, multiplicand.real);
  mpz_mul(rightPart, multiplier.imaginary, multiplicand.imaginary);
  mpz_sub(r, leftPart, rightPart);
  mpz_mod(r, r, modulus);

  mpz_mul(leftPart, multiplier.imaginary, multiplicand.real);
  mpz_mul(rightPart, multiplier.real, multiplicand.imaginary);
  mpz_add(i, leftPart, rightPart);
  mpz_mod(i, i, modulus);

  complex_initMpz(product, r, i);
  mpz_clears(r, i, leftPart, rightPart, NULL);
}

void complex_modPow(Complex *power, const Complex base, const mpz_t exponent,
                    const mpz_t modulus) {
  if (!mpz_cmp_ui(modulus, 1)) {
    complex_initLong(power, 0, 0);
    return;
  }

  mpz_t baseRealCopy, baseImaginaryCopy, expCopy, expMod;
  mpz_inits(baseRealCopy, baseImaginaryCopy, expCopy, expMod, NULL);

  complex_initLong(power, 1, 0);

  mpz_set(baseRealCopy, base.real);
  mpz_mod(baseRealCopy, baseRealCopy, modulus);

  mpz_set(baseImaginaryCopy, base.imaginary);
  mpz_mod(baseImaginaryCopy, baseImaginaryCopy, modulus);

  Complex baseCopy;
  complex_initMpz(&baseCopy, baseRealCopy, baseImaginaryCopy);

  mpz_set(expCopy, exponent);

  while (mpz_cmp_ui(expCopy, 0) > 0) {
    mpz_mod_ui(expMod, expCopy, 2);
    if (!mpz_cmp_ui(expMod, 1)) {
      Complex tmp;
      complex_modMul(&tmp, baseCopy, *power, modulus);
      complex_destroy(*power);
      *power = tmp;
    }

    mpz_fdiv_q_2exp(expCopy, expCopy, 1);

    Complex tmp;
    complex_modMul(&tmp, baseCopy, baseCopy, modulus);
    complex_destroy(baseCopy);
    baseCopy = tmp;
  }

  complex_destroy(baseCopy);
  mpz_clears(baseRealCopy, baseImaginaryCopy, expCopy, expMod, NULL);
}

void complex_modMulInteger(Complex *product, const mpz_t multiplier, const Complex multiplicand,
                          const mpz_t modulus) {
  // Calculated as
  // \f$(r \cdot s \mod m, i \cdot s \mod m)\f$.
  mpz_t r, imag;
  mpz_inits(r, imag, NULL);

  mpz_mul(r, multiplier, multiplicand.real);
  mpz_mod(r, r, modulus);

  mpz_mul(imag, multiplier, multiplicand.imaginary);
  mpz_mod(imag, imag, modulus);

  complex_initMpz(product, r, imag);
  mpz_clears(r, imag, NULL);
}

// The inverse of z is z^{-1} = \frac{1}{z} =
// \frac{r}{r^2+i^2}-\frac{i}{r^2+i^2}
CryptidStatus complex_multiplicativeInverse(Complex *inverse,
                                            const Complex operand,
                                            const mpz_t modulus) {
  mpz_t real, imaginary, denom, realSquare, imagSquare, denomInv, negImag;
  ;

  // (0, 0) has no multiplicative inverse.
  if (!mpz_cmp_ui(operand.real, 0) && !mpz_cmp_ui(operand.imaginary, 0)) {
    return CRYPTID_HAS_NO_MUL_INV_ERROR;
  }

  mpz_inits(real, imaginary, denom, realSquare, imagSquare, denomInv, negImag,
            NULL);

  // If the Complex instance only holds a real value, we can fallback to
  // simple inverse: \f$(r^{-1}, 0)\f$.
  if (!mpz_cmp_ui(operand.imaginary, 0)) {
    mpz_invert(real, operand.real, modulus);
    complex_initMpzLong(inverse, real, 0);

    mpz_clears(real, imaginary, denom, realSquare, imagSquare, denomInv,
               negImag, NULL);
    return CRYPTID_SUCCESS;
  }

  // Likewise, if the Complex instance only holds an imaginary value, we
  // can simplify things: \f$(0, -i^{-1})\f$.
  if (!mpz_cmp_ui(operand.real, 0)) {
    mpz_invert(imaginary, operand.imaginary, modulus);
    mpz_neg(imaginary, imaginary);
    mpz_mod(imaginary, imaginary, modulus);
    complex_initLongMpz(inverse, 0, imaginary);

    mpz_clears(real, imaginary, denom, realSquare, imagSquare, denomInv,
               negImag, NULL);
    return CRYPTID_SUCCESS;
  }

  mpz_pow_ui(realSquare, operand.real, 2);
  mpz_pow_ui(imagSquare, operand.imaginary, 2);
  mpz_add(denom, realSquare, imagSquare);
  mpz_mod(denom, denom, modulus);

  mpz_invert(denomInv, denom, modulus);

  mpz_mul(real, operand.real, denomInv);
  mpz_mod(real, real, modulus);

  mpz_neg(negImag, operand.imaginary);
  mpz_mod(negImag, negImag, modulus);

  mpz_mul(imaginary, negImag, denomInv);
  mpz_mod(imaginary, imaginary, modulus);

  complex_initMpz(inverse, real, imaginary);
  mpz_clears(real, imaginary, denom, realSquare, imagSquare, denomInv, negImag,
             NULL);
  return CRYPTID_SUCCESS;
}
