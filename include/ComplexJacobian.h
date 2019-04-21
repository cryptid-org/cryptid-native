#ifndef COMPLEXJACOBIAN_H
#define COMPLEXJACOBIAN_H

#include "gmp.h"

#include "Complex.h"
#include "ComplexAffinePoint.h"
#include "EllipticCurve.h"
#include "Status.h"

typedef struct ComplexJacobian
{
    Complex x;
    Complex y;
    Complex z;
} ComplexJacobian;

ComplexJacobian complexJacobian_init(Complex x, Complex y, Complex z);

ComplexJacobian complexJacobian_initLong(long xr, long xi, long yr, long yi, long zr, long zi);

void complexJacobian_destroy(ComplexJacobian jacobian);

ComplexJacobian complexJacobian_infinity();

int complexJacobian_isEquals(ComplexJacobian jacobian1, ComplexJacobian jacobian2);

int complexJacobian_isInfinity(ComplexJacobian jacobian);

ComplexJacobian complexAffine_toComplexJacobian(ComplexAffinePoint affinePoint);

Status complexJacobian_toComplexAffine(ComplexAffinePoint *result, ComplexJacobian jacobian, EllipticCurve ellipticCurve);

ComplexJacobian complexJacobian_double(ComplexJacobian jacobian, EllipticCurve ellipticCurve);

ComplexJacobian complexJacobian_add(ComplexJacobian jacobian1, ComplexJacobian jacobian2, EllipticCurve ellipticCurve);

ComplexJacobian complexJacobian_multiply(mpz_t s, ComplexJacobian jacobian, EllipticCurve ellipticCurve);

#endif
