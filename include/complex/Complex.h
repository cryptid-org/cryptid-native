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
 * Returns a new Complex number initialized to (0, 0).
 * @return (0,0)
 */
Complex complex_init(void);

/**
 * Returns a new Complex initialized with the specified arbitrary precision values.
 * @param real the real part
 * @param imaginary the imaginary part
 * @return a new complex
 */
Complex complex_initMpz(const mpz_t real, const mpz_t imaginary);

/**
 * Returns a new Complex initialized with the specified long values.
 * @param real the real part
 * @param imaginary the imaginary part
 * @return a new complex
 */
Complex complex_initLong(const long real, const long imaginary);

/**
 * Returns a new Complex initialized with an arbitrary precision real part and a long imaginary part.
 * @param real the real part
 * @param imaginary the imaginary part
 * @return a new complex.
 */
Complex complex_initMpzLong(const mpz_t real, const long imaginary);

/**
 * Returns a new Complex initialized with a long real part and ann arbitrary precision imaginary part.
 * @param real the real part
 * @param imaginary the imaginary part
 * @return a new complex.
 */
Complex complex_initLongMpz(const long real, const mpz_t imaginary);

/**
 * Checks if two Complex instances hold the same values.
 * @param complex1 a Complex to check
 * @param complex2 a Complex to check
 * @return 1 if the two Complex instances are equal, 0 otherwise
 */
int complex_isEquals(const Complex complex1, const Complex complex2);

/**
 * Frees a Complex instance. After calling this function on a Complex instance, that instance 
 * should not be used anymore.
 * @param complex the Complex to be destroyed
 */
void complex_destroy(const Complex complex);

/**
 * Frees multiple Complex instances. After calling this function on a Complex instance, that instance
 * should not be used anymore.
 * @param argumentCount the count of instances to be destroyed
 */
void complex_destroyMany(const size_t argumentCount, ...);

/**
 * Adds two Complex valus with respect to the specified modulus.
 * @param complex1 a value to add
 * @param complex2 a value to add
 * @param p the modulus
 * @return the result of the addition
 */
Complex complex_modAdd(const Complex complex1, const Complex complex2, const mpz_t p);

/**
 * Calculates the additive inverse of the specified Complex with respect to the specified modulus.
 * @param complex the Complex to invert
 * @param p the modulus
 * @return the additive inverse
 */
Complex complex_additiveInverse(const Complex complex, const mpz_t p);

/**
 * Adds a Complex and an integer with respect to the specified modulus.
 * @param complex a Complex
 * @param s a scalar
 * @param p the modulus
 * @return the result of the addition
 */
Complex complex_modAddScalar(const Complex complex, const  mpz_t s, const mpz_t p);

/**
 * Multiplies two Complex values with respect to the specified modulus.
 * @param complex1 a Complex to multiply
 * @param complex2 a Complex to multiply
 * @param p the modulus.
 * @return the result of the multiplication.
 */
Complex complex_modMul(const Complex complex1, const Complex complex2, const mpz_t p);

/**
 * Raised a Complex value to the specified exponent modulo p.
 * @param complex the base
 * @param exp the exponent
 * @param p the modulus
 * @return the result of the exponentiation
 */
Complex complex_modPow(const Complex complex, const mpz_t exp, const mpz_t p);

/**
 * Multiplies a Complex value with an integer.
 * @param complex a Complex
 * @param s the scalar.
 * @param p the modulus.
 * @return the result of the multiplication.
 */
Complex complex_modMulScalar(const Complex complex, const mpz_t s, const mpz_t p);

/**
 * Calculates the multiplicate inverse of a Complex with respect to p.
 * @param result Out parameter to the multiplicative inverse. On SUCCESS, this should be destroyed by the caller.
 * @param complex a Complex.
 * @param p the modulus.
 * @return 0 if complex has a multiplicative inverse, HAS_NO_MUL_INV error otherwise
 */
Status complex_multiplicativeInverse(Complex *result, const Complex complex, const mpz_t p);

#endif