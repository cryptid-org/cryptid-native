#include <stdlib.h>
#include <string.h>

#include "elliptic/ComplexAffinePoint.h"


// References:
//   * [Guide-to-ECC] Darrel Hankerson, Alfred J. Menezes, and Scott Vanstone. 2010. Guide to Elliptic Curve Cryptography (1st ed.). Springer Publishing Company, Incorporated.
//   * [Intro-to-IBE] Luther Martin. 2008. Introduction to Identity-Based Encryption (Information Security and Privacy Series) (1 ed.). Artech House, Inc., Norwood, MA, USA. 


void complexAffine_init(ComplexAffinePoint *complexAffinePointOutput, const Complex x, const Complex y)
{
    complex_initMpz(&complexAffinePointOutput->x, x.real, x.imaginary);
    complex_initMpz(&complexAffinePointOutput->y, y.real, y.imaginary);
}

void complexAffine_initLong(ComplexAffinePoint *complexAffinePointOutput, const long xr, const long xi, const long yr, const long yi)
{
    complex_initLong(&complexAffinePointOutput->x, xr, xi);
    complex_initLong(&complexAffinePointOutput->y, yr, yi);
}

void complexAffine_destroy(ComplexAffinePoint complexAffinePoint)
{
    complex_destroyMany(2, complexAffinePoint.x, complexAffinePoint.y);
}

ComplexAffinePoint complexAffine_infinity(void)
{
    ComplexAffinePoint infinity;

    complexAffine_initLong(&infinity, -1, 0, -1, 0);

    return infinity;
}

int complexAffine_isEquals(const ComplexAffinePoint complexAffinePoint1, const ComplexAffinePoint complexAffinePoint2)
{
    return complex_isEquals(complexAffinePoint1.x, complexAffinePoint2.x) && complex_isEquals(complexAffinePoint1.y, complexAffinePoint2.y);
}

int complexAffine_isInfinity(const ComplexAffinePoint complexAffinePoint)
{
    ComplexAffinePoint infty = complexAffine_infinity();

    int result = complexAffine_isEquals(infty, complexAffinePoint);
    complexAffine_destroy(infty);

    return result;
}

CryptidStatus complexAffine_double(ComplexAffinePoint *result, const ComplexAffinePoint complexAffinePoint, const EllipticCurve ellipticCurve)
{
    // Double-only implementation of Algorithm 3.1 in [Intro-to-IBE].

    // Doubling infinity yields infinity.
    if(complexAffine_isInfinity(complexAffinePoint))
    {
        *result = complexAffine_infinity();
        return CRYPTID_SUCCESS;
    }

    Complex complexZero;
    complex_initLong(&complexZero, 0, 0);

    // If the \f$y\f$ coordinate is equal to complex zero, then the result is infinity.
    if(complex_isEquals(complexAffinePoint.y, complexZero))
    {
        complex_destroy(complexZero);
        *result = complexAffine_infinity();
        return CRYPTID_SUCCESS;
    }

    complex_destroy(complexZero);

    mpz_t tmp;
    mpz_init_set_ui(tmp, 2);
    Complex denom;

    // See Equation 3.4 in [Intro-to-IBE].
    // \f$\frac{3x^{2} + a}{2y}
    Complex twoTimesAp1y;
    complex_modMulScalar(&twoTimesAp1y, complexAffinePoint.y, tmp, ellipticCurve.fieldOrder);

    // If \f$2y\f$ has no multiplicative inverse, the above expression cannot be calculated.
    CryptidStatus status = complex_multiplicativeInverse(&denom, twoTimesAp1y, ellipticCurve.fieldOrder);
    if(status)
    {
        complex_destroy(twoTimesAp1y);
        mpz_clear(tmp);
        return status;
    }

    complex_destroy(twoTimesAp1y);

    mpz_set_ui(tmp, 3);

    Complex ap1xSquared;
    complex_modMul(&ap1xSquared, complexAffinePoint.x, complexAffinePoint.x, ellipticCurve.fieldOrder);
    Complex threeTimesAp1xSquared;
    complex_modMulScalar(&threeTimesAp1xSquared, ap1xSquared, tmp, ellipticCurve.fieldOrder);
    Complex num;
    complex_modAddScalar(&num, threeTimesAp1xSquared, ellipticCurve.a, ellipticCurve.fieldOrder);

    Complex m;
    complex_modMul(&m, num, denom, ellipticCurve.fieldOrder);

    complex_destroyMany(4, num, threeTimesAp1xSquared, ap1xSquared, denom);
    mpz_clear(tmp);

    // Same as in {@code complexAffine_add}.
    // \f$x_n = m^{2}-2x\f$
    Complex x2AddInv;
    complex_additiveInverse(&x2AddInv, complexAffinePoint.x, ellipticCurve.fieldOrder);
    Complex mSquared;
    complex_modMul(&mSquared, m, m, ellipticCurve.fieldOrder);

    Complex x1AddInv;
    complex_additiveInverse(&x1AddInv, complexAffinePoint.x, ellipticCurve.fieldOrder);

    Complex x1AddInvPlusx2AddInv;
    complex_modAdd(&x1AddInvPlusx2AddInv, x1AddInv, x2AddInv, ellipticCurve.fieldOrder);

    Complex xn;
    complex_modAdd(&xn, mSquared, x1AddInvPlusx2AddInv, ellipticCurve.fieldOrder);

    // \f$y_n = m(x - x_n) - y\f$
    Complex xAddInv;
    complex_additiveInverse(&xAddInv, xn, ellipticCurve.fieldOrder);
    Complex q;
    complex_modAdd(&q, complexAffinePoint.x, xAddInv, ellipticCurve.fieldOrder);
    Complex r;
    complex_modMul(&r, m, q, ellipticCurve.fieldOrder);

    Complex y1AddInv;
    complex_additiveInverse(&y1AddInv, complexAffinePoint.y, ellipticCurve.fieldOrder);
    Complex yn;
    complex_modAdd(&yn, r, y1AddInv, ellipticCurve.fieldOrder);

    complexAffine_init(result, xn, yn);


    complex_destroyMany(11, yn, r, q, xAddInv, xn, x1AddInvPlusx2AddInv, mSquared, x2AddInv, m, x1AddInv, y1AddInv);

    return CRYPTID_SUCCESS;
}

CryptidStatus complexAffine_add(ComplexAffinePoint *result, const ComplexAffinePoint complexAffinePoint1,
                         const ComplexAffinePoint complexAffinePoint2, EllipticCurve ellipticCurve)
{
    // Implementation of Algorithm 3.1 in [Intro-to-IBE].

    // Adding infinity to a point does not change the point. 
    if(complexAffine_isInfinity(complexAffinePoint1))
    {
        complexAffine_init(result, complexAffinePoint2.x, complexAffinePoint2.y);
        return CRYPTID_SUCCESS;
    }

    if(complexAffine_isInfinity(complexAffinePoint2))
    {
        complexAffine_init(result, complexAffinePoint1.x, complexAffinePoint1.y);
        return CRYPTID_SUCCESS;
    }

    // If the points are equal to each other, we can speed things up
    // by performing a point doubling instead of an addition.
    if(complexAffine_isEquals(complexAffinePoint1, complexAffinePoint2))
    {
        complexAffine_double(result, complexAffinePoint1, ellipticCurve);
        return CRYPTID_SUCCESS;
    }

    // Having equal \f$x\f$ coordinates (and different points) a divide-by-zero error would
    // happen, thus we return infinity.
    // Note, that in the algorithm, this check is the first step, however, that's wrong.
    if(complex_isEquals(complexAffinePoint1.x, complexAffinePoint2.x))
    {
        *result = complexAffine_infinity();
        return CRYPTID_SUCCESS;
    }


    Complex x1AddInv;
    complex_additiveInverse(&x1AddInv, complexAffinePoint1.x, ellipticCurve.fieldOrder);

    // \f$\frac{y_2 - y_1}{x_2 - x_1}\f$
    Complex ap2xPlusx1AddInv;
    complex_modAdd(&ap2xPlusx1AddInv, complexAffinePoint2.x, x1AddInv, ellipticCurve.fieldOrder);

    Complex denom;
    CryptidStatus status = complex_multiplicativeInverse(&denom, ap2xPlusx1AddInv, ellipticCurve.fieldOrder);
    if(status)
    {
        complex_destroyMany(2, x1AddInv, ap2xPlusx1AddInv);
        return status;
    }

    complex_destroy(ap2xPlusx1AddInv);

    Complex y1AddInv;
    complex_additiveInverse(&y1AddInv, complexAffinePoint1.y, ellipticCurve.fieldOrder);
    Complex num;
    complex_modAdd(&num, complexAffinePoint2.y, y1AddInv, ellipticCurve.fieldOrder);

    Complex m;
    complex_modMul(&m, num, denom, ellipticCurve.fieldOrder);

    complex_destroyMany(2, denom, num);

    // \f$x_n = m^{2}-x_1-x_2\f$
    Complex x2AddInv;
    complex_additiveInverse(&x2AddInv, complexAffinePoint2.x, ellipticCurve.fieldOrder);
    Complex mSquared;
    complex_modMul(&mSquared, m, m, ellipticCurve.fieldOrder);

    Complex x1AddInvPlusx2AddInv;
    complex_modAdd(&x1AddInvPlusx2AddInv, x1AddInv, x2AddInv, ellipticCurve.fieldOrder);

    Complex xn;
    complex_modAdd(&xn, mSquared, x1AddInvPlusx2AddInv, ellipticCurve.fieldOrder);

    // \f$y_n = m(x_1 - x_3) - y_n\f$
    Complex xAddInv;
    complex_additiveInverse(&xAddInv, xn, ellipticCurve.fieldOrder);
    Complex q;
    complex_modAdd(&q, complexAffinePoint1.x, xAddInv, ellipticCurve.fieldOrder);
    Complex r;
    complex_modMul(&r, m, q, ellipticCurve.fieldOrder);

    Complex yn;
    complex_modAdd(&yn, r, y1AddInv, ellipticCurve.fieldOrder);

    complexAffine_init(result, xn, yn);


    complex_destroyMany(11, yn, r, q, xAddInv, xn, x1AddInvPlusx2AddInv, mSquared, x2AddInv, m, x1AddInv, y1AddInv);

    return CRYPTID_SUCCESS;
}

CryptidStatus complexAffine_multiply(ComplexAffinePoint *result, const mpz_t s, const ComplexAffinePoint complexAffinePoint,
                              const EllipticCurve ellipticCurve)
{
    // Implementation of Algorithm 3.26 in [Guide-to-ECC].

    mpz_t zero;
    mpz_init_set_ui(zero, 0);

    // Multiplication by zero yields infinity.
    if(!mpz_cmp(s, zero))
    {
        mpz_clear(zero);
        *result = complexAffine_infinity();
        return CRYPTID_SUCCESS;
    }

    // Multiplying infinity yields infinity.
    if(complexAffine_isInfinity(complexAffinePoint))
    {
        mpz_clear(zero);
        *result = complexAffine_infinity();
        return CRYPTID_SUCCESS;
    }

    ComplexAffinePoint pointN;
    complexAffine_init(&pointN, complexAffinePoint.x, complexAffinePoint.y);
    // \f$Q = \infty\f$
    ComplexAffinePoint pointQ = complexAffine_infinity();

    // Binary expansion of the multiplier.
    char *d = mpz_get_str(NULL, 2, s);

    // Right-to-left iteration
    for(int i = strlen(d) - 1; i >= 0; i--)
    {
        CryptidStatus status;

        // If \f$k_i = 1\f$ then \f$Q = Q + P\f$.
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

        // \f$P = 2P\f$
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
    return CRYPTID_SUCCESS;
}

int complexAffine_isOnCurve(ComplexAffinePoint point, EllipticCurve ellipticCurve)
{
    // Check if
    // \f$y^2\f$
    // is equal to
    // \f$x^3 + ax + b\f$.
    Complex ySquared;
    complex_modMul(&ySquared, point.y, point.y, ellipticCurve.fieldOrder);
    Complex xSquared;
    complex_modMul(&xSquared, point.x, point.x, ellipticCurve.fieldOrder);
    Complex xCubed;
    complex_modMul(&xCubed, xSquared, point.x, ellipticCurve.fieldOrder);

    Complex ax;
    complex_modMulScalar(&ax, point.x, ellipticCurve.a, ellipticCurve.fieldOrder);

    Complex xCubedPlusAx;
    complex_modAdd(&xCubedPlusAx, xCubed, ax, ellipticCurve.fieldOrder);

    mpz_t zero;
    mpz_init_set_ui(zero, 0);

    Complex b;
    complex_initMpz(&b, ellipticCurve.b, zero);

    Complex rhs;
    complex_modAdd(&rhs, xCubedPlusAx, b, ellipticCurve.fieldOrder);

    int result = complex_isEquals(ySquared, rhs);

    mpz_clear(zero);
    complex_destroyMany(7, ySquared, xSquared, xCubed, ax, xCubedPlusAx, b, rhs);

    return result;
}
