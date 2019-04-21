#ifndef ELLIPTICCURVE_H
#define ELLIPTICCURVE_H

#include "gmp.h"

typedef struct EllipticCurve
{
    mpz_t a;
    mpz_t b;
    mpz_t fieldOrder;
} EllipticCurve;

EllipticCurve ellipticCurve_init(mpz_t a, mpz_t b, mpz_t fieldOrder);

EllipticCurve ellipticCurve_initLong(long a, long b, long fieldOrder);

void ellipticCurve_destroy(EllipticCurve ellipticCurve);

#endif
