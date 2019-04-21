#include <stdarg.h>

#include "complex/Complex.h"


Complex complex_init(void)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    return complex;
} 

Complex complex_initMpz(mpz_t real, mpz_t imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set(complex.real, real);
    mpz_set(complex.imaginary, imaginary);

    return complex;
}

Complex complex_initLong(long real, long imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set_si(complex.real, real);
    mpz_set_si(complex.imaginary, imaginary);

    return complex;
}

Complex complex_initMpzLong(mpz_t real, long imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set(complex.real, real);
    mpz_set_si(complex.imaginary, imaginary);

    return complex;
}

Complex complex_initLongMpz(long real, mpz_t imaginary)
{
    Complex complex;

    mpz_inits(complex.real, complex.imaginary, NULL);

    mpz_set_si(complex.real, real);
    mpz_set(complex.imaginary, imaginary);

    return complex;
}

int complex_isEquals(Complex complex1, Complex complex2)
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

void complex_destroyMany(size_t argumentCount, ...)
{
    va_list args;
    va_start(args, argumentCount);

    for (size_t i = 0; i < argumentCount; ++i) {
        Complex complex = (va_arg(args, Complex));

        mpz_clears(complex.real, complex.imaginary, NULL);
    }

    va_end(args);
}

Complex complex_modAdd(Complex complex1, Complex complex2, mpz_t p)
{
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

Complex complex_additiveInverse(Complex complex, mpz_t p)
{
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

Complex complex_modAddScalar(Complex complex, mpz_t s, mpz_t p)
{
    Complex result;
    mpz_t newReal;
    mpz_init(newReal);

    mpz_add(newReal, complex.real, s);
    mpz_mod(newReal, newReal, p);

    result = complex_initMpz(newReal, complex.imaginary);
    mpz_clear(newReal);
    return result;
}

Complex complex_modMul(Complex complex1, Complex complex2, mpz_t p)
{
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

Complex complex_modPow(Complex complex, mpz_t exp, mpz_t p)
{
    mpz_t baseRealCopy, baseImaginaryCopy, expCopy, expMod;
    mpz_inits(baseRealCopy, baseImaginaryCopy, expCopy, expMod, NULL);

    if(!mpz_cmp_ui(p, 1))
    {
        mpz_clears(baseRealCopy, baseImaginaryCopy, expCopy, expMod, NULL);
        return complex_initLong(0, 0);
    }

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

Complex complex_modMulScalar(Complex complex, mpz_t s, mpz_t p)
{
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

Status complex_multiplicativeInverse(Complex *result, Complex complex, mpz_t p)
{
    mpz_t real, imaginary, realInv, q, r, rInv, t;
    mpz_inits(real, imaginary, realInv, q, r, rInv, t, NULL);

    if(!mpz_cmp_ui(complex.real, 0) && !mpz_cmp_ui(complex.imaginary, 0))
    {
        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return HAS_NO_MUL_INV_ERROR;
    }

    if(!mpz_cmp_ui(complex.imaginary, 0))
    {
        mpz_invert(real, complex.real, p);
        *result = complex_initMpzLong(real, 0);
        
        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return SUCCESS;
    }

    if(!mpz_cmp_ui(complex.real, 0))
    {
        mpz_invert(imaginary, complex.imaginary, p);
        mpz_neg(imaginary, imaginary);
        mpz_mod(imaginary, imaginary, p);
        *result = complex_initLongMpz(0, imaginary);

        mpz_clears(real, imaginary, realInv, q, r, rInv, t, NULL);
        return SUCCESS;
    }

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
