#include <stdlib.h>
#include <string.h>

#include "ComplexJacobian.h"

ComplexJacobian complexJacobian_init(Complex x, Complex y, Complex z)
{
    ComplexJacobian jacobian;
    jacobian.x = complex_initMpz(x.real, x.imaginary);
    jacobian.y = complex_initMpz(y.real, y.imaginary);
    jacobian.z = complex_initMpz(z.real, z.imaginary);

    return jacobian;
}

ComplexJacobian complexJacobian_initLong(long xr, long xi, long yr, long yi, long zr, long zi)
{
    ComplexJacobian jacobian;
    jacobian.x = complex_initLong(xr, xi);
    jacobian.y = complex_initLong(yr, yi);
    jacobian.z = complex_initLong(zr, zi);

    return jacobian;
}

void complexJacobian_destroy(ComplexJacobian jacobian)
{
    complex_destroyMany(3, jacobian.x, jacobian.y, jacobian.z);
}

ComplexJacobian complexJacobian_infinity()
{
    return complexJacobian_initLong(0, 0, 1, 0, 0, 0);
}

int complexJacobian_isEquals(ComplexJacobian jacobian1, ComplexJacobian jacobian2)
{
    return complex_isEquals(jacobian1.x, jacobian2.x)
        && complex_isEquals(jacobian1.y, jacobian2.y)
        && complex_isEquals(jacobian1.z, jacobian2.z);
}

int complexJacobian_isInfinity(ComplexJacobian jacobian)
{
    ComplexJacobian infty = complexJacobian_infinity();

    int result = complexJacobian_isEquals(infty, jacobian);
    complexJacobian_destroy(infty);

    return result;
}

ComplexJacobian complexAffine_toComplexJacobian(ComplexAffinePoint affinePoint)
{
    if(complexAffine_isInfinity(affinePoint))
    {
        return complexJacobian_infinity();
    }

    Complex one = complex_initLong(1, 0);

    ComplexJacobian result = complexJacobian_init(affinePoint.x, affinePoint.y, one);

    complex_destroy(one);
    return result;
}

Status complexJacobian_toComplexAffine(ComplexAffinePoint *result, ComplexJacobian jacobian, EllipticCurve ellipticCurve)
{
    if(complexJacobian_isInfinity(jacobian))
    {
        *result = complexAffine_infinity();
        return SUCCESS;
    }

    mpz_t two, three;
    mpz_inits(two, three, NULL);
    mpz_set_ui(two, 2);
    mpz_set_ui(three, 3);

    Complex zSquared = complex_modMul(jacobian.z, jacobian.z, ellipticCurve.fieldOrder);
    Complex denom;
    Status status = complex_multiplicativeInverse(&denom, zSquared, ellipticCurve.fieldOrder);
    if(status)
    {
        mpz_clears(two, three, NULL);
        complex_destroy(zSquared);
        return status;
    }
    Complex x = complex_modMul(jacobian.x, denom, ellipticCurve.fieldOrder);

    Complex zPowThree = complex_modPow(jacobian.z, three, ellipticCurve.fieldOrder);
    complex_destroy(denom);
    status = complex_multiplicativeInverse(&denom, zPowThree, ellipticCurve.fieldOrder);
    if(status)
    {
        mpz_clears(two, three, NULL);
        complex_destroyMany(3, zSquared, x, zPowThree);
        return status;
    }
    Complex y = complex_modMul(jacobian.y, denom, ellipticCurve.fieldOrder);

    *result = complexAffine_init(x, y);

    mpz_clears(two, three, NULL);
    complex_destroyMany(5, zSquared, x, zPowThree, y, denom);
    return SUCCESS;
}

ComplexJacobian complexJacobian_double(ComplexJacobian jacobian, EllipticCurve ellipticCurve)
{
    Complex zero = complex_initLong(0, 0);
    if(complex_isEquals(jacobian.y, zero))
    {
        complex_destroy(zero);
        return complexJacobian_infinity();
    }

    mpz_t four, three, two, eight;
    mpz_inits(four, three, two, eight, NULL);

    Complex ySquared = complex_modMul(jacobian.y, jacobian.y, ellipticCurve.fieldOrder);
    Complex xMulySquared = complex_modMul(jacobian.x, ySquared, ellipticCurve.fieldOrder);
    mpz_set_ui(four, 4);
    Complex s = complex_modMulScalar(xMulySquared, four, ellipticCurve.fieldOrder);

    //For type-1 elliptic curve (we use those.)
    Complex xSquared = complex_modMul(jacobian.x, jacobian.x, ellipticCurve.fieldOrder);
    mpz_set_ui(three, 3);
    Complex m = complex_modMulScalar(xSquared, three, ellipticCurve.fieldOrder);

    //For y^2 = x^3 + ax + b elliptic curves
    //Complex xSquared = complex_modMul(jacobian.x, jacobian.x, ellipticCurve.fieldOrder);
    //mpz_set_ui(three, 3);
    //Complex xSquaredMulThree = complex_modMulScalar(xSquared, three, ellipticCurve.fieldOrder);
    //Complex zPowFour = complex_modPow(jacobian.z, four, ellipticCurve.fieldOrder);
    //Complex aMulzPowFour = = complex_modMulScalar(zPowFour, ellipticCurve.a, ellipticCurve.fieldOrder);
    //Complex m = complex_modAdd(xSquaredMulThree, aMulzPowFour, ellipticCurve.fieldOrder);
    //complex_destoy(xSquaredMulThree);
    //complex_destoy(zPowFour);
    //complex_destoy(aMulzPowFour);

    Complex mSquared = complex_modMul(m, m, ellipticCurve.fieldOrder);
    mpz_set_ui(two, 2);
    Complex sMulTwo = complex_modMulScalar(s, two, ellipticCurve.fieldOrder);
    Complex sMulTwoAddInv = complex_additiveInverse(sMulTwo, ellipticCurve.fieldOrder);
    Complex x2 = complex_modAdd(mSquared, sMulTwoAddInv, ellipticCurve.fieldOrder);

    Complex x2AddInv = complex_additiveInverse(x2, ellipticCurve.fieldOrder);
    Complex sAddx2AddInv = complex_modAdd(s, x2AddInv, ellipticCurve.fieldOrder);
    Complex mMulsAddx2AddInv = complex_modMul(m, sAddx2AddInv, ellipticCurve.fieldOrder);
    mpz_set_ui(eight, 8);
    Complex yPowFour = complex_modPow(jacobian.y, four, ellipticCurve.fieldOrder);
    Complex yPowFourMulEight = complex_modMulScalar(yPowFour, eight, ellipticCurve.fieldOrder);
    Complex yPowFourMulEightAddInv = complex_additiveInverse(yPowFourMulEight, ellipticCurve.fieldOrder);
    Complex y2 = complex_modAdd(mMulsAddx2AddInv, yPowFourMulEightAddInv, ellipticCurve.fieldOrder);

    Complex yMulTwo = complex_modMulScalar(jacobian.y, two, ellipticCurve.fieldOrder);
    Complex z2 = complex_modMul(yMulTwo, jacobian.z, ellipticCurve.fieldOrder);

    ComplexJacobian result = complexJacobian_init(x2, y2, z2);
    mpz_clears(four, three, two, eight, NULL);

    complex_destroyMany(19, zero, ySquared, xMulySquared, s, xSquared, m, mSquared, sMulTwo, sMulTwoAddInv, x2, 
                        x2AddInv, sAddx2AddInv, mMulsAddx2AddInv, yPowFour, yPowFourMulEight, yPowFourMulEightAddInv, y2, yMulTwo,
                        z2);
    return result;
}

ComplexJacobian complexJacobian_add(ComplexJacobian jacobian1, ComplexJacobian jacobian2, EllipticCurve ellipticCurve)
{
    if(complexJacobian_isInfinity(jacobian1))
    {
        return complexJacobian_init(jacobian2.x, jacobian2.y, jacobian2.z);
    }

    if(complexJacobian_isInfinity(jacobian2))
    {
        return complexJacobian_init(jacobian1.x, jacobian1.y, jacobian1.z);
    }

    mpz_t three, two;
    mpz_inits(three, two, NULL);

    Complex z2Squared = complex_modMul(jacobian2.z, jacobian2.z, ellipticCurve.fieldOrder);
    Complex u1 = complex_modMul(jacobian1.x, z2Squared, ellipticCurve.fieldOrder);

    Complex z1Squared = complex_modMul(jacobian1.z, jacobian1.z, ellipticCurve.fieldOrder);
    Complex u2 = complex_modMul(jacobian2.x, z1Squared, ellipticCurve.fieldOrder);

    mpz_set_ui(three, 3);
    Complex z2PowThree = complex_modPow(jacobian2.z, three, ellipticCurve.fieldOrder);
    Complex s1 = complex_modMul(jacobian1.y, z2PowThree, ellipticCurve.fieldOrder);

    Complex z1PowThree = complex_modPow(jacobian1.z, three, ellipticCurve.fieldOrder);
    Complex s2 = complex_modMul(jacobian2.y, z1PowThree, ellipticCurve.fieldOrder);

    complex_destroyMany(4, z2Squared, z1Squared, z2PowThree, z1PowThree);
    if(complex_isEquals(u1, u2))
    {
        complex_destroyMany(2, u1, u2);
        mpz_clears(three, two, NULL);
        if(!complex_isEquals(s1, s2))
        {
            complex_destroyMany(2, s1, s2);
            return complexJacobian_infinity();
        }
        else
        {
            complex_destroyMany(2, s1, s2);
            return complexJacobian_double(jacobian1, ellipticCurve);
        }
    }

    Complex u1AddInv = complex_additiveInverse(u1, ellipticCurve.fieldOrder);
    Complex h = complex_modAdd(u2, u1AddInv, ellipticCurve.fieldOrder);

    Complex s1AddInv = complex_additiveInverse(s1, ellipticCurve.fieldOrder);
    Complex r = complex_modAdd(s2, s1AddInv, ellipticCurve.fieldOrder);

    Complex rSquared = complex_modMul(r, r, ellipticCurve.fieldOrder);
    Complex hPowThree = complex_modPow(h, three, ellipticCurve.fieldOrder);
    Complex hPowThreeAddInv = complex_additiveInverse(hPowThree, ellipticCurve.fieldOrder);
    mpz_set_ui(two, 2);
    Complex u1MulTwo = complex_modMulScalar(u1, two, ellipticCurve.fieldOrder);
    Complex hSquared = complex_modMul(h, h, ellipticCurve.fieldOrder);
    Complex u1MulTwoMulhSquared = complex_modMul(u1MulTwo, hSquared, ellipticCurve.fieldOrder);
    Complex u1MulTwoMulhSquaredAddInv = complex_additiveInverse(u1MulTwoMulhSquared, ellipticCurve.fieldOrder);
    Complex rSquaredAddhPowThreeAddInv = complex_modAdd(rSquared, hPowThreeAddInv, ellipticCurve.fieldOrder);
    Complex x3 = complex_modAdd(rSquaredAddhPowThreeAddInv, u1MulTwoMulhSquaredAddInv, ellipticCurve.fieldOrder);

    Complex u1MulhSquared = complex_modMul(u1, hSquared, ellipticCurve.fieldOrder);
    Complex x3AddInv = complex_additiveInverse(x3, ellipticCurve.fieldOrder);
    Complex u1MulhSquaredAddx3AddInv = complex_modAdd(u1MulhSquared, x3AddInv, ellipticCurve.fieldOrder);
    Complex rMulu1MulhSquaredAddx3AddInv = complex_modMul(r, u1MulhSquaredAddx3AddInv, ellipticCurve.fieldOrder);
    Complex s1MulhPowThree = complex_modMul(s1, hPowThree, ellipticCurve.fieldOrder);
    Complex s1MulhPowThreeAddInv = complex_additiveInverse(s1MulhPowThree, ellipticCurve.fieldOrder);
    Complex y3 = complex_modAdd(rMulu1MulhSquaredAddx3AddInv, s1MulhPowThreeAddInv, ellipticCurve.fieldOrder);

    Complex hMulz1 = complex_modMul(h, jacobian1.z, ellipticCurve.fieldOrder);
    Complex z3 = complex_modMul(hMulz1, jacobian2.z, ellipticCurve.fieldOrder);

    ComplexJacobian result = complexJacobian_init(x3, y3, z3);

    mpz_clears(three, two, NULL);

    complex_destroyMany(26, u1, u2, s1, s2, u1AddInv, h, s1AddInv, r, rSquared, hPowThree, hPowThreeAddInv, u1MulTwo, hSquared,
                        u1MulTwoMulhSquared, u1MulTwoMulhSquaredAddInv, rSquaredAddhPowThreeAddInv, x3, u1MulhSquared, x3AddInv,
                        u1MulhSquaredAddx3AddInv, rMulu1MulhSquaredAddx3AddInv, s1MulhPowThree, s1MulhPowThreeAddInv, y3, hMulz1, z3);

    return result;
}

ComplexJacobian complexJacobian_multiply(mpz_t s, ComplexJacobian jacobian, EllipticCurve ellipticCurve)
{
    if(!mpz_cmp_ui(s, 0))
    {
        return complexJacobian_infinity();
    }

    if(complexJacobian_isInfinity(jacobian))
    {
        return complexJacobian_infinity();
    }

    ComplexJacobian jacobianN = complexJacobian_init(jacobian.x, jacobian.y, jacobian.z);
    ComplexJacobian jacobianQ = complexJacobian_infinity();

    char *d = mpz_get_str(NULL, 2, s);

    for(int i = strlen(d) - 1; i >= 0; i--)
    {
        if(d[i] == '1')
        {
            ComplexJacobian tmp = complexJacobian_add(jacobianQ, jacobianN, ellipticCurve);
            complexJacobian_destroy(jacobianQ);
            jacobianQ = tmp;
        }

        ComplexJacobian tmp = complexJacobian_double(jacobianN, ellipticCurve);
        complexJacobian_destroy(jacobianN);
        jacobianN = tmp;
    }

    free(d);

    complexJacobian_destroy(jacobianN);

    return jacobianQ;
}
