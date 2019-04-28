#ifndef __CRYPTID_JACOBIAN_H
#define __CRYPTID_JACOBIAN_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"


typedef struct Jacobian
{
    mpz_t x;
    mpz_t y;
    mpz_t z;
} Jacobian;

Jacobian jacobian_init(mpz_t x, mpz_t y, mpz_t z);

Jacobian jacobian_initLong(long x, long y, long z);

void jacobian_destroy(Jacobian jacobian);

Jacobian jacobian_infinity();

int jacobian_isEquals(Jacobian jacobian1, Jacobian jacobian2);

int jacobian_isInfinity(Jacobian jacobian);

Jacobian affine_toJacobian(AffinePoint affinePoint);

Status jacobian_toAffine(AffinePoint *result, Jacobian jacobian, EllipticCurve ellipticCurve);

Jacobian jacobian_double(Jacobian jacobian, EllipticCurve ellipticCurve);

Jacobian jacobian_add(Jacobian jacobian1, Jacobian jacobian2, EllipticCurve ellipticCurve);

Jacobian jacobian_multiply(mpz_t s, Jacobian jacobian, EllipticCurve ellipticCurve);

#endif
