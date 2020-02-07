#ifndef __CRYPTID_ELLIPTICCURVE_H
#define __CRYPTID_ELLIPTICCURVE_H

#include "gmp.h"


/**
 * A finite field on an elliptic curve of form \f$y^2 = x^3 + ax + b\f$.
 */
typedef struct EllipticCurve
{
    mpz_t a;
    mpz_t b;
    mpz_t fieldOrder;
} EllipticCurve;

/**
 * Returns a new EllipticCurve instance initialized with arbitrary precision integers.
 * @param ellipticCurveOutput the EllipticCurve to be initialized
 * @param a coefficient \f$a\f$
 * @param b coefficient \f$b\f$
 * @param fieldOrder the order of the finite field
 */
void ellipticCurve_init(EllipticCurve *ellipticCurveOutput, const mpz_t a, const mpz_t b, const mpz_t fieldOrder);

/**
 * Returns a new EllipticCurve instance initialized longs.
 * @param ellipticCurveOutput the EllipticCurve to be initialized
 * @param a coefficient \f$a\f$
 * @param b coefficient \f$b\f$
 * @param fieldOrder the order of the finite field
 */
void ellipticCurve_initLong(EllipticCurve *ellipticCurveOutput, const long a, const long b, const long fieldOrder);

/**
 * Frees an EllipticCurve.
 * @param ellipticCurve the EllipticCurve to be destroyed
 */
void ellipticCurve_destroy(EllipticCurve ellipticCurve);

#endif
