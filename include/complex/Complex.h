#ifndef __CRYPTID_COMPLEX_H
#define __CRYPTID_COMPLEX_H

#include <stddef.h>

#include "gmp.h"

#include "util/Status.h"


/**
 * Arbitrary precision complex number with integer-only real and imaginary parts.
 */
typedef struct Complex
{
    /**
     * The real part of the complex number.
     */
    mpz_t real;

    /**
     * The imaginary part of the complex number
     */
    mpz_t imaginary;
} Complex;

/**
 * Returns a new Complex number init'd to (0, 0).
 * @return (0,0)
 */
Complex complex_init(void);

/**
 * Returns a new Complex initialized with the specified arbitrary precision values.
 * @param real the real part
 * @param imaginary the imaginary part
 * @return a new complex
 */
Complex complex_initMpz(mpz_t real, mpz_t imaginary);

/**
 * Returns a new Complex initialized with the specified long values.
 * @param real the real part
 * @param imaginary the imaginary part
 * @return a new complex
 */
Complex complex_initLong(long real, long imaginary);

Complex complex_initMpzLong(mpz_t real, long imaginary);

Complex complex_initLongMpz(long real, mpz_t imaginary);

/**
 * Checks if two Complex instances hold the same values.
 * @param complex1 a Complex to check
 * @param complex2 a Complex to check
 * @return 1 if the two Complex instances are equal, 0 otherwise
 */
int complex_isEquals(Complex complex1, Complex complex2);

/**
 * Frees a Complex instance. After calling this function on a Complex instance, that instance should not be used anymore.
 * @param complex the Complex to be destroyed
 */
void complex_destroy(Complex complex);

/**
 * Frees multiple Complex instances. After calling this function on a Complex instance, that instance should not be used anymore.
 * @param argumentCount the count of instances to be destroyed
 */
void complex_destroyMany(size_t argumentCount, ...);

/**
 * Returns a Complex whose value is {@code ((complex1.real + complex2.real) mod p, (complex1.imaginary + complex2.imaginary) mod p)}.
 * @param complex1 a value to add
 * @param complex2 a value to add
 * @param p the modulus.
 * @return the result of the addition
 */
Complex complex_modAdd(Complex complex1, Complex complex2, mpz_t p);

/**
 * Returns a Complex that is the additive inverse of the specified Complex with respect to the specified modulus.
 * @param complex the Complex to invert
 * @param p the modulus
 * @return the additive inverse
 */
Complex complex_additiveInverse(Complex complex, mpz_t p);

/**
 * Returns a Complex whose value is {@code ((complex.real + s) mod p, complex.imaginry)}.
 * @param complex a Complex
 * @param s a scalar
 * @param p the modulus
 * @return the result of the addition
 */
Complex complex_modAddScalar(Complex complex, mpz_t s, mpz_t p);

/**
 * Returns a Complex whose value is
 * {@code ((complex1.real * complex2.real - complex1.imag * complex2.imag) mod p, 
 *         (complex1.imag * complex2.real + complex1.real * complex2.imag) mod p)}.
 * @param complex1 a Complex to multiply
 * @param complex2 a Complex to multiply
 * @param p the modulus.
 * @return the result of the multiplication.
 */
Complex complex_modMul(Complex complex1, Complex complex2, mpz_t p);

/**
 * Returns a Complex whose value is the sepcified Complex raised to the specified exponent modulo p.
 * @param complex the base
 * @param exp the exponent
 * @param p the modulus
 * @return the result of the exponentiation
 */
Complex complex_modPow(Complex complex, mpz_t exp, mpz_t p);

/**
 * Returns a Complex whose value is {@code ((complex.real * s) mod p, (complex.imag * s) mod p)}.
 * @param complex a Complex
 * @param s the scalar.
 * @param p the modulus.
 * @return the result of the multiplication.
 */
Complex complex_modMulScalar(Complex complex, mpz_t s, mpz_t p);

/**
 * Calculates the multiplicate inverse of a Complex with respect to p.
 * @param result A pointer to a Complex in which the result will be stored. The pointed instance should NOT be initialized.
 * @param complex a Complex.
 * @param p the modulus.
 * @return 0 if complex has a multiplicative inverse, error otherwise
 */
Status complex_multiplicativeInverse(Complex *result, Complex complex, mpz_t p);

#endif