#include <stdarg.h>

#include "complex/Complex.h"


Complex complex_init(void)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    return complex;
} 

Complex complex_initMpz(const mpz_t real, const mpz_t imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set(complex.real, real);
    mpz_set(complex.imaginary, imaginary);

    return complex;
}

Complex complex_initLong(const long real, const long imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set_si(complex.real, real);
    mpz_set_si(complex.imaginary, imaginary);

    return complex;
}

Complex complex_initMpzLong(const mpz_t real, const long imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set(complex.real, real);
    mpz_set_si(complex.imaginary, imaginary);

    return complex;
}

Complex complex_initLongMpz(const long real, const mpz_t imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set_si(complex.real, real);
    mpz_set(complex.imaginary, imaginary);

    return complex;
}

int complex_isEquals(const Complex complex1, const Complex complex2)
{
    if(!mpz_cmp(complex1.real, complex2.real) && !mpz_cmp(complex1.imaginary, complex2.imaginary))
    {
        return 1;
    }

    return 0;
}

void complex_destroy(Complex complex)
{
    mpz_clears(complex.real, complex.imaginary, NULL);
}

void complex_destroyMany(const size_t argumentCount, ...)
{
    va_list args;
    va_start(args, argumentCount);

    for (size_t i = 0; i < argumentCount; ++i) {
        Complex complex = (va_arg(args, Complex));

        mpz_clears(complex.real, complex.imaginary, NULL);
    }

    va_end(args);
}

Complex complex_modAdd(const Complex complex1, const Complex complex2, const mpz_t p)
{
    // Calculated as
    // \f$(r_1 + r_2 \mod p, c_1 + c_2 \mod p)\f$.
    Complex result;
    mpz_t newReal, newImaginary;
    mpz_inits(newReal, newImaginary, NULL);

    mpz_add(newReal, complex1.real, complex2.real);
    mpz_mod(newReal, newReal, p);

    mpz_add(newImaginary, complex1.imaginary, complex2.imaginary);
    mpz_mod(newImaginary, newImaginary, p);

    result = complex_initMpz(newReal, newImaginary);
    mpz_clears(newReal, newImaginary, NULL);
    return result;
}

Complex complex_additiveInverse(const Complex complex, const mpz_t p)
{
    // Calculated as
    // \f$(-r \mod p, -i \mod p)\f$.
    Complex result;
    mpz_t newReal, newImaginary;
    mpz_inits(newReal, newImaginary, NULL);

    mpz_neg(newReal, complex.real);
    mpz_mod(newReal, newReal, p);

    mpz_neg(newImaginary, complex.imaginary);
    mpz_mod(newImaginary, newImaginary, p);

    result = complex_initMpz(newReal, newImaginary);
    mpz_clears(newReal, newImaginary, NULL);
    return result;
}

Complex complex_modAddScalar(const Complex complex, const mpz_t s, const mpz_t p)
{
    // Calculated as
    // \f$(r + s \mod p, i)\f$.
    Complex result;
    mpz_t newReal;
    mpz_init(newReal);

    mpz_add(newReal, complex.real, s);
    mpz_mod(newReal, newReal, p);

    result = complex_initMpz(newReal, complex.imaginary);
    mpz_clear(newReal);
    return result;
}

Complex complex_modMul(const Complex complex1, const Complex complex2, const mpz_t p)
{
    // Calculated as
    // \f$((r_1 \cdot r_2 - i_1 \cdot i_2) \mod p, (i_1 \cdot r_2 + r_1 \cdot i_2) \mod p)\f$.
    Complex result;
    mpz_t r, i, leftPart, rightPart;
    mpz_inits(r, i, leftPart, rightPart, NULL);

    mpz_mul(leftPart, complex1.real, complex2.real);
    mpz_mul(rightPart, complex1.imaginary, complex2.imaginary);
    mpz_sub(r, leftPart, rightPart);
    mpz_mod(r, r, p);

    mpz_mul(leftPart, complex1.imaginary, complex2.real);
    mpz_mul(rightPart, complex1.real, complex2.imaginary);
    mpz_add(i, leftPart, rightPart);
    mpz_mod(i, i, p);

    result = complex_initMpz(r, i);
    mpz_clears(r, i, leftPart, rightPart, NULL);
    return result;
}

Complex complex_modPow(const Complex complex, const mpz_t exp, const mpz_t p)
{
    if(!mpz_cmp_ui(p, 1))
    {
        return complex_initLong(0, 0);
    }

    mpz_t baseRealCopy, baseImaginaryCopy, expCopy, expMod;
    mpz_inits(baseRealCopy, baseImaginaryCopy, expCopy, expMod, NULL);

    Complex result = complex_initLong(1, 0);

    mpz_set(baseRealCopy, complex.real);
    mpz_mod(baseRealCopy, baseRealCopy, p);

    mpz_set(baseImaginaryCopy, complex.imaginary);
    mpz_mod(baseImaginaryCopy, baseImaginaryCopy, p);

    Complex baseCopy = complex_initMpz(baseRealCopy, baseImaginaryCopy);

    mpz_set(expCopy, exp);

    while(mpz_cmp_ui(expCopy, 0) > 0)
    {
        mpz_mod_ui(expMod, expCopy, 2);
        if(!mpz_cmp_ui(expMod, 1))
        {
            Complex tmp = complex_modMul(baseCopy, result, p);
            complex_destroy(result);
            result = tmp;
        }

        mpz_fdiv_q_2exp(expCopy, expCopy, 1);

        Complex tmp = complex_modMul(baseCopy, baseCopy, p);
        complex_destroy(baseCopy);
        baseCopy = tmp;
    }

    complex_destroy(baseCopy);
    mpz_clears(baseRealCopy, baseImaginaryCopy, expCopy, expMod, NULL);

    return result;
}

Complex complex_modMulScalar(const Complex complex, const mpz_t s, const mpz_t p)
{
    // Calculated as
    // \f$(r \cdot s \mod p, i \cdot s \mod p)\f$.
    Complex result;
    mpz_t r, imag;
    mpz_inits(r, imag, NULL);

    mpz_mul(r, s, complex.real);
    mpz_mod(r, r, p);

    mpz_mul(imag, s, complex.imaginary);
    mpz_mod(imag, imag, p);

    result = complex_initMpz(r, imag);
    mpz_clears(r, imag, NULL);
    return result;
}

Status complex_multiplicativeInverse(Complex *result, const Complex complex, const mpz_t p)
{
    mpz_t real, imaginary, realInv, q, r, rInv, t;
    mpz_inits(real, imaginary, realInv, q, r, rInv, t, NULL);

    // (0, 0) has no multiplicative inverse.
    if(!mpz_cmp_ui(complex.real, 0) && !mpz_cmp_ui(complex.imaginary, 0))
    {
        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return HAS_NO_MUL_INV_ERROR;
    }

    // If the Complex instance only holds a real value, we can fallback to
    // simple inverse: \f$(r^{-1}, 0)\f$.
    if(!mpz_cmp_ui(complex.imaginary, 0))
    {
        mpz_invert(real, complex.real, p);
        *result = complex_initMpzLong(real, 0);
        
        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return SUCCESS;
    }

    // Likewise, if the Complex instance only holds an imaginary value, we
    // can simplify things: \f$(0, -i^{-1})\f$.
    if(!mpz_cmp_ui(complex.real, 0))
    {
        mpz_invert(imaginary, complex.imaginary, p);
        mpz_neg(imaginary, imaginary);
        mpz_mod(imaginary, imaginary, p);
        *result = complex_initLongMpz(0, imaginary);

        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return SUCCESS;
    }

    // Otherwise, we have a long way to go...
    // Note, that both the real and the imaginary parts are guaranteed to be non-zero.

    // First we try to calculate the imaginary part (denoted as {@code y}).
    // \f$y =  -(i \cdot r^{-1}) \cdot (r + r^{-1} \cdot i^{2})^{-1}\f$
    //                                  |
    // This sum can be zero. If that's the case, then the number has no inverse.
    // The terms of the product are denoted as {@code q} and {@code rInv}.
    mpz_invert(realInv, complex.real, p);

    mpz_neg(q, complex.imaginary);
    mpz_mod(q, q, p);
    mpz_mul(q, realInv, q);

    mpz_pow_ui(r, complex.imaginary, 2);
    mpz_mul(r, realInv, r);
    mpz_add(r, complex.real, r);

    if(!mpz_cmp_ui(r, 0))
    {
        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return HAS_NO_MUL_INV_ERROR;
    }

    // {@code x} (the real part) can be calculated using {@code y}.
    // \f$x = r^{-1} + y \cdot r^{-1} \cdot i\f$
    // The second term of the sum is denoted as {@code t}.
    mpz_invert(rInv, r, p);

    mpz_mul(imaginary, q, rInv);
    mpz_mod(imaginary, imaginary, p);

    mpz_mul(t, imaginary, realInv);
    mpz_mul(t, t, complex.imaginary);
    mpz_mod(t, t, p);

    mpz_add(real, realInv, t);
    mpz_mod(real, real, p);

    *result = complex_initMpz(real, imaginary);
    mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
    return SUCCESS;
}
