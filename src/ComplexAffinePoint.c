#include <stdlib.h>
#include <string.h>

#include "ComplexAffinePoint.h"

ComplexAffinePoint complexAffine_init(Complex x, Complex y)
{
    ComplexAffinePoint complexAffinePoint;
    complexAffinePoint.x = complex_initMpz(x.real, x.imaginary);
    complexAffinePoint.y = complex_initMpz(y.real, y.imaginary);
    return complexAffinePoint;
}

ComplexAffinePoint complexAffine_initLong(long xr, long xi, long yr, long yi)
{
    ComplexAffinePoint complexAffinePoint;
    complexAffinePoint.x = complex_initLong(xr, xi);
    complexAffinePoint.y = complex_initLong(yr, yi);

    return complexAffinePoint;
}

void complexAffine_destroy(ComplexAffinePoint complexAffinePoint)
{
    complex_destroyMany(2, complexAffinePoint.x, complexAffinePoint.y);
}

ComplexAffinePoint complexAffine_infinity()
{
    return complexAffine_initLong(-1, 0, -1, 0);
}

int complexAffine_isEquals(ComplexAffinePoint complexAffinePoint1, ComplexAffinePoint complexAffinePoint2)
{
    return complex_isEquals(complexAffinePoint1.x, complexAffinePoint2.x) && complex_isEquals(complexAffinePoint1.y, complexAffinePoint2.y);
}

int complexAffine_isInfinity(ComplexAffinePoint complexAffinePoint)
{
    ComplexAffinePoint infty = complexAffine_infinity();

    int result = complexAffine_isEquals(infty, complexAffinePoint);
    complexAffine_destroy(infty);

    return result;
}

Status complexAffine_double(ComplexAffinePoint *result, ComplexAffinePoint complexAffinePoint, EllipticCurve ellipticCurve)
{
    if(complexAffine_isInfinity(complexAffinePoint))
    {
        *result = complexAffine_infinity();
        return SUCCESS;
    }

    Complex complexZero = complex_initLong(0, 0);
    Complex x1AddInv = complex_additiveInverse(complexAffinePoint.x, ellipticCurve.fieldOrder);
    Complex y1AddInv = complex_additiveInverse(complexAffinePoint.y, ellipticCurve.fieldOrder);

    if(complex_isEquals(complexAffinePoint.y, complexZero))
    {
        complex_destroyMany(3, complexZero, x1AddInv, y1AddInv);
        *result = complexAffine_infinity();
        return SUCCESS;
    }

    complex_destroy(complexZero);

    Complex m;

    mpz_t tmp;
    mpz_init_set_ui(tmp, 2);
    Complex denom;

    Complex twoTimesAp1y = complex_modMulScalar(complexAffinePoint.y, tmp, ellipticCurve.fieldOrder);

    Status status = complex_multiplicativeInverse(&denom, twoTimesAp1y, ellipticCurve.fieldOrder);
    if(status)
    {
        complex_destroyMany(3, x1AddInv, y1AddInv, twoTimesAp1y);
        mpz_clear(tmp);
        return status;
    }

    complex_destroy(twoTimesAp1y);

    mpz_set_ui(tmp, 3);

    Complex ap1xSquared = complex_modMul(complexAffinePoint.x, complexAffinePoint.x, ellipticCurve.fieldOrder);
    Complex threeTimesAp1xSquared = complex_modMulScalar(ap1xSquared, tmp, ellipticCurve.fieldOrder);
    Complex num = complex_modAddScalar(threeTimesAp1xSquared, ellipticCurve.a, ellipticCurve.fieldOrder);

    m = complex_modMul(num, denom, ellipticCurve.fieldOrder);

    complex_destroyMany(4, num, threeTimesAp1xSquared, ap1xSquared, denom);
    mpz_clear(tmp);

    Complex x2AddInv = complex_additiveInverse(complexAffinePoint.x, ellipticCurve.fieldOrder);
    Complex mSquared = complex_modMul(m, m, ellipticCurve.fieldOrder);

    Complex x1AddInvPlusx2AddInv = complex_modAdd(x1AddInv, x2AddInv, ellipticCurve.fieldOrder);

    Complex xn = complex_modAdd(mSquared, x1AddInvPlusx2AddInv, ellipticCurve.fieldOrder);

    Complex xAddInv = complex_additiveInverse(xn, ellipticCurve.fieldOrder);
    Complex q = complex_modAdd(complexAffinePoint.x, xAddInv, ellipticCurve.fieldOrder);
    Complex r = complex_modMul(m, q, ellipticCurve.fieldOrder);

    Complex yn = complex_modAdd(r, y1AddInv, ellipticCurve.fieldOrder);

    *result = complexAffine_init(xn, yn);


    complex_destroyMany(11, yn, r, q, xAddInv, xn, x1AddInvPlusx2AddInv, mSquared, x2AddInv, m, x1AddInv, y1AddInv);

    return SUCCESS;
}

Status complexAffine_add(ComplexAffinePoint *result, ComplexAffinePoint complexAffinePoint1, ComplexAffinePoint complexAffinePoint2, EllipticCurve ellipticCurve)
{
    if(complexAffine_isInfinity(complexAffinePoint1))
    {
        *result = complexAffine_init(complexAffinePoint2.x, complexAffinePoint2.y);
        return SUCCESS;
    }

    if(complexAffine_isInfinity(complexAffinePoint2))
    {
        *result = complexAffine_init(complexAffinePoint1.x, complexAffinePoint1.y);
        return SUCCESS;
    }

    Complex m;

    Complex x1AddInv = complex_additiveInverse(complexAffinePoint1.x, ellipticCurve.fieldOrder);
    Complex y1AddInv = complex_additiveInverse(complexAffinePoint1.y, ellipticCurve.fieldOrder);

    if(complexAffine_isEquals(complexAffinePoint1, complexAffinePoint2))
    {
        complexAffine_double(result, complexAffinePoint1, ellipticCurve);
        return SUCCESS;
    }
    else
    {
        if(complex_isEquals(complexAffinePoint1.x, complexAffinePoint2.x))
        {
            complex_destroyMany(2, x1AddInv, y1AddInv);
            *result = complexAffine_infinity();
            return SUCCESS;
        }

        Complex denom;

        Complex ap2xPlusx1AddInv = complex_modAdd(complexAffinePoint2.x, x1AddInv, ellipticCurve.fieldOrder);

        Status status = complex_multiplicativeInverse(&denom, ap2xPlusx1AddInv, ellipticCurve.fieldOrder);
        if(status)
        {
            complex_destroyMany(3, x1AddInv, y1AddInv, ap2xPlusx1AddInv);
            return status;
        }

        complex_destroy(ap2xPlusx1AddInv);

        Complex num =  complex_modAdd(complexAffinePoint2.y, y1AddInv, ellipticCurve.fieldOrder);

        m = complex_modMul(num, denom, ellipticCurve.fieldOrder);

        complex_destroyMany(2, denom, num);
    }

    Complex x2AddInv = complex_additiveInverse(complexAffinePoint2.x, ellipticCurve.fieldOrder);
    Complex mSquared = complex_modMul(m, m, ellipticCurve.fieldOrder);

    Complex x1AddInvPlusx2AddInv = complex_modAdd(x1AddInv, x2AddInv, ellipticCurve.fieldOrder);

    Complex xn = complex_modAdd(mSquared, x1AddInvPlusx2AddInv, ellipticCurve.fieldOrder);

    Complex xAddInv = complex_additiveInverse(xn, ellipticCurve.fieldOrder);
    Complex q = complex_modAdd(complexAffinePoint1.x, xAddInv, ellipticCurve.fieldOrder);
    Complex r = complex_modMul(m, q, ellipticCurve.fieldOrder);

    Complex yn = complex_modAdd(r, y1AddInv, ellipticCurve.fieldOrder);

    *result = complexAffine_init(xn, yn);


    complex_destroyMany(11, yn, r, q, xAddInv, xn, x1AddInvPlusx2AddInv, mSquared, x2AddInv, m, x1AddInv, y1AddInv);

    return SUCCESS;
}

Status complexAffine_multiply(ComplexAffinePoint *result, mpz_t s, ComplexAffinePoint complexAffinePoint, EllipticCurve ellipticCurve)
{
    mpz_t zero;
    mpz_init_set_ui(zero, 0);

    if(!mpz_cmp(s, zero))
    {
        mpz_clear(zero);
        *result = complexAffine_infinity();
        return SUCCESS;
    }

    if(complexAffine_isInfinity(complexAffinePoint))
    {
        mpz_clear(zero);
        *result = complexAffine_infinity();
        return SUCCESS;
    }

    ComplexAffinePoint pointN = complexAffine_init(complexAffinePoint.x, complexAffinePoint.y);
    ComplexAffinePoint pointQ = complexAffine_infinity();

    char *d = mpz_get_str(NULL, 2, s);

    for(int i = strlen(d) - 1; i >= 0; i--)
    {
        Status status;

        if(d[i] == '1')
        {
            ComplexAffinePoint tmp;
            status = complexAffine_add(&tmp, pointQ, pointN, ellipticCurve);
            if(status)
            {
                mpz_clear(zero);
                complexAffine_destroy(pointN);
                complexAffine_destroy(pointQ);
                free(d);
                return status;
            }
            complexAffine_destroy(pointQ);
            pointQ = tmp;
        }

        ComplexAffinePoint tmp;
        status = complexAffine_double(&tmp, pointN, ellipticCurve);
        if(status)
        {
            mpz_clear(zero);
            complexAffine_destroy(pointN);
            complexAffine_destroy(pointQ);
            free(d);
            return status;
        }
        complexAffine_destroy(pointN);
        pointN = tmp;
    }

    free(d);

    complexAffine_destroy(pointN);

    mpz_clear(zero);
    *result = pointQ;
    return SUCCESS;
}

int complexAffine_isOnCurve(ComplexAffinePoint point, EllipticCurve ellipticCurve)
{
    Complex ySquared = complex_modMul(point.y, point.y, ellipticCurve.fieldOrder);
    Complex xSquared = complex_modMul(point.x, point.x, ellipticCurve.fieldOrder);
    Complex xCubed = complex_modMul(xSquared, point.x, ellipticCurve.fieldOrder);

    Complex ax = complex_modMulScalar(point.x, ellipticCurve.a, ellipticCurve.fieldOrder);

    Complex xCubedPlusAx = complex_modAdd(xCubed, ax, ellipticCurve.fieldOrder);

    mpz_t zero;
    mpz_init_set_ui(zero, 0);

    Complex b = complex_initMpz(ellipticCurve.b, zero);

    Complex rhs = complex_modAdd(xCubedPlusAx, b, ellipticCurve.fieldOrder);

    int result = complex_isEquals(ySquared, rhs);

    mpz_clear(zero);
    complex_destroyMany(7, ySquared, xSquared, xCubed, ax, xCubedPlusAx, b, rhs);

    return result;
}
