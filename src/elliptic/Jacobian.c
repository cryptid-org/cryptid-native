#include <stdlib.h>
#include <string.h>

#include "elliptic/Jacobian.h"


Jacobian jacobian_init(mpz_t x, mpz_t y, mpz_t z)
{
    Jacobian jacobian;
    mpz_inits(jacobian.x, jacobian.y, jacobian.z, NULL);
    mpz_set(jacobian.x, x);
    mpz_set(jacobian.y, y);
    mpz_set(jacobian.z, z);

    return jacobian;
}

Jacobian jacobian_initLong(long x, long y, long z)
{
    Jacobian jacobian;
    mpz_inits(jacobian.x, jacobian.y, jacobian.z, NULL);
    mpz_set_si(jacobian.x, x);
    mpz_set_si(jacobian.y, y);
    mpz_set_si(jacobian.z, z);

    return jacobian;
}

void jacobian_destroy(Jacobian jacobian)
{
    mpz_clears(jacobian.x, jacobian.y, jacobian.z, NULL);
}

Jacobian jacobian_infinity()
{
    return jacobian_initLong(0, 1, 0);
}

int jacobian_isEquals(Jacobian jacobian1, Jacobian jacobian2)
{
    if(!mpz_cmp(jacobian1.x, jacobian2.x)
        && !mpz_cmp(jacobian1.y, jacobian2.y)
        && !mpz_cmp(jacobian1.z, jacobian2.z))
    {
        return 1;
    }

    return 0;
}

int jacobian_isInfinity(Jacobian jacobian)
{
    Jacobian infty = jacobian_infinity();

    int result = jacobian_isEquals(infty, jacobian);
    jacobian_destroy(infty);

    return result;
}

Jacobian affine_toJacobian(AffinePoint affinePoint)
{
    if(affine_isInfinity(affinePoint))
    {
        return jacobian_infinity();
    }

    mpz_t one;
    mpz_init_set_ui(one, 1);

    Jacobian result = jacobian_init(affinePoint.x, affinePoint.y, one);

    mpz_clear(one);
    return result;
}

Status jacobian_toAffine(AffinePoint *result, Jacobian jacobian, EllipticCurve ellipticCurve)
{
    if(jacobian_isInfinity(jacobian))
    {
        *result = affine_infinity();
        return SUCCESS;
    }

    mpz_t zSquared, denomX, x, zPowThree, denomY, y;
    mpz_inits(zSquared, denomX, x, zPowThree, denomY, y, NULL);

    mpz_powm_ui(zSquared, jacobian.z, 2, ellipticCurve.fieldOrder);
    mpz_invert(denomX, zSquared, ellipticCurve.fieldOrder);
    mpz_mul(x, jacobian.x, denomX);
    mpz_mod(x, x, ellipticCurve.fieldOrder);

    mpz_powm_ui(zPowThree, jacobian.z, 3, ellipticCurve.fieldOrder);
    mpz_invert(denomY, zPowThree, ellipticCurve.fieldOrder);
    mpz_mul(y, jacobian.y, denomY);
    mpz_mod(y, y, ellipticCurve.fieldOrder);

    *result = affine_init(x, y);

    mpz_clears(zSquared, denomX, x, zPowThree, denomY, y, NULL);
    return SUCCESS;
}

Jacobian jacobian_double(Jacobian jacobian, EllipticCurve ellipticCurve)
{
    if(!mpz_cmp_ui(jacobian.y, 0))
    {
        return jacobian_infinity();
    }

    mpz_t ySquared, xMulySquared, s, xSquared, m,
        mSquared, sMulTwo, x2, sSubx2, mMulsSubx2, yPowFour,
        yPowFourMulEight, y2, yMulTwo, z2;
    
    mpz_inits(ySquared, xMulySquared, s, xSquared, m,
        mSquared, sMulTwo, x2, sSubx2, mMulsSubx2, yPowFour,
        yPowFourMulEight, y2, yMulTwo, z2, NULL);

    mpz_powm_ui(ySquared, jacobian.y, 2, ellipticCurve.fieldOrder);
    mpz_mul(xMulySquared, jacobian.x, ySquared);
    mpz_mul_ui(s, xMulySquared, 4);
    mpz_mod(s, s, ellipticCurve.fieldOrder);

    //For type-1 elliptic curve (we use those.)
    mpz_powm_ui(xSquared, jacobian.x, 2, ellipticCurve.fieldOrder);
    mpz_mul_ui(m, xSquared, 3);
    mpz_mod(m, m, ellipticCurve.fieldOrder);

    //For y^2 = x^3 + ax + b elliptic curves
    //mpz_t xSquaredMulThree, zPowFour, aMulzPowFour;
    //mpz_inits(xSquaredMulThree, zPowFour, aMulzPowFour, NULL);
    //mpz_powm_ui(xSquared, jacobian.x, 2, ellipticCurve.fieldOrder);
    //mpz_mul_ui(xSquaredMulThree, xSquared, 3);
    //mpz_powm_ui(zPowFour, jacobian.z, 4, ellipticCurve.fieldOrder);
    //mpz_mul(aMulzPowFour, ellipticCurve.a, zPowFour);
    //mpz_add(m, xSquaredMulThree, aMulzPowFour);
    //mpz_mod(m, m, ellipticCurve.fieldOrder);
    //mpz_clears(xSquaredMulThree, zPowFour, aMulzPowFour, NULL);

    mpz_powm_ui(mSquared, m, 2, ellipticCurve.fieldOrder);
    mpz_mul_ui(sMulTwo, s, 2);
    mpz_mod(sMulTwo, sMulTwo, ellipticCurve.fieldOrder);
    mpz_sub(x2, mSquared, sMulTwo);
    mpz_mod(x2, x2, ellipticCurve.fieldOrder);

    mpz_sub(sSubx2, s, x2);
    mpz_mod(sSubx2, sSubx2, ellipticCurve.fieldOrder);
    mpz_mul(mMulsSubx2, m, sSubx2);
    mpz_mod(mMulsSubx2, mMulsSubx2, ellipticCurve.fieldOrder);
    mpz_powm_ui(yPowFour, jacobian.y, 4, ellipticCurve.fieldOrder);
    mpz_mul_ui(yPowFourMulEight, yPowFour, 8);
    mpz_mod(yPowFourMulEight, yPowFourMulEight, ellipticCurve.fieldOrder);
    mpz_sub(y2, mMulsSubx2, yPowFourMulEight);
    mpz_mod(y2, y2, ellipticCurve.fieldOrder);

    mpz_mul_ui(yMulTwo, jacobian.y, 2);
    mpz_mul(z2, yMulTwo, jacobian.z);
    mpz_mod(z2, z2, ellipticCurve.fieldOrder);

    Jacobian result = jacobian_init(x2, y2, z2);
    mpz_clears(ySquared, xMulySquared, s, xSquared, m,
        mSquared, sMulTwo, x2, sSubx2, mMulsSubx2, yPowFour,
        yPowFourMulEight, y2, yMulTwo, z2, NULL);

    return result;
}

Jacobian jacobian_add(Jacobian jacobian1, Jacobian jacobian2, EllipticCurve ellipticCurve)
{
    if(jacobian_isInfinity(jacobian1))
    {
        return jacobian_init(jacobian2.x, jacobian2.y, jacobian2.z);
    }

    if(jacobian_isInfinity(jacobian2))
    {
        return jacobian_init(jacobian1.x, jacobian1.y, jacobian1.z);
    }

    mpz_t z2Squared, u1, z1Squared, u2, z2PowThree, s1, z1PowThree, s2;
    mpz_inits(z2Squared, u1, z1Squared, u2, z2PowThree, s1, z1PowThree, s2, NULL);

    mpz_powm_ui(z2Squared, jacobian2.z, 2, ellipticCurve.fieldOrder);
    mpz_mul(u1, jacobian1.x, z2Squared);
    mpz_mod(u1, u1, ellipticCurve.fieldOrder);

    mpz_powm_ui(z1Squared, jacobian1.z, 2, ellipticCurve.fieldOrder);
    mpz_mul(u2, jacobian2.x, z1Squared);
    mpz_mod(u2, u2, ellipticCurve.fieldOrder);

    mpz_powm_ui(z2PowThree, jacobian2.z, 3, ellipticCurve.fieldOrder);
    mpz_mul(s1, jacobian1.y, z2PowThree);
    mpz_mod(s1, s1, ellipticCurve.fieldOrder);

    mpz_powm_ui(z1PowThree, jacobian1.z, 3, ellipticCurve.fieldOrder);
    mpz_mul(s2, jacobian2.y, z1PowThree);
    mpz_mod(s2, s2, ellipticCurve.fieldOrder);

    mpz_clears(z2Squared, z1Squared, z2PowThree, z1PowThree, NULL);
    if(!mpz_cmp(u1, u2))
    {
        mpz_clears(u1, u2, NULL);
        if(mpz_cmp(s1, s2))
        {
            mpz_clears(s1, s2, NULL);
            return jacobian_infinity();
        }
        else
        {
            mpz_clears(s1, s2, NULL);
            return jacobian_double(jacobian1, ellipticCurve);
        }
    }

    mpz_t h, r, rSquared, hPowThree, u1MulTwo, hSquared,
        u1MulTwoMulhSquared, rSquaredSubhPowThree, x3,
        u1MulhSquared, u1MulhSquaredSubx3, rMulu1MulhSquaredSubx3,
        s1MulhPowThree, y3, hMulz1, z3;
    mpz_inits(h, r, rSquared, hPowThree, u1MulTwo, hSquared,
        u1MulTwoMulhSquared, rSquaredSubhPowThree, x3,
        u1MulhSquared, u1MulhSquaredSubx3, rMulu1MulhSquaredSubx3,
        s1MulhPowThree, y3, hMulz1, z3, NULL);

    mpz_sub(h, u2, u1);
    mpz_mod(h, h, ellipticCurve.fieldOrder);

    mpz_sub(r, s2, s1);
    mpz_mod(r, r, ellipticCurve.fieldOrder);

    mpz_powm_ui(rSquared, r, 2, ellipticCurve.fieldOrder);
    mpz_powm_ui(hPowThree, h, 3, ellipticCurve.fieldOrder);
    mpz_mul_ui(u1MulTwo, u1, 2);
    mpz_powm_ui(hSquared, h, 2, ellipticCurve.fieldOrder);
    mpz_mul(u1MulTwoMulhSquared, u1MulTwo, hSquared);
    mpz_sub(rSquaredSubhPowThree, rSquared, hPowThree);
    mpz_mod(rSquaredSubhPowThree, rSquaredSubhPowThree, ellipticCurve.fieldOrder);
    mpz_sub(x3, rSquaredSubhPowThree, u1MulTwoMulhSquared);
    mpz_mod(x3, x3, ellipticCurve.fieldOrder);

    mpz_mul(u1MulhSquared, u1, hSquared);
    mpz_sub(u1MulhSquaredSubx3, u1MulhSquared, x3);
    mpz_mod(u1MulhSquaredSubx3, u1MulhSquaredSubx3, ellipticCurve.fieldOrder);
    mpz_mul(rMulu1MulhSquaredSubx3, r, u1MulhSquaredSubx3);
    mpz_mul(s1MulhPowThree, s1, hPowThree);
    mpz_sub(y3, rMulu1MulhSquaredSubx3, s1MulhPowThree);
    mpz_mod(y3, y3, ellipticCurve.fieldOrder);

    mpz_mul(hMulz1, h, jacobian1.z);
    mpz_mul(z3, hMulz1, jacobian2.z);
    mpz_mod(z3, z3, ellipticCurve.fieldOrder);

    Jacobian result = jacobian_init(x3, y3, z3);

    mpz_clears(h, r, rSquared, hPowThree, u1MulTwo, hSquared,
        u1MulTwoMulhSquared, rSquaredSubhPowThree, x3,
        u1MulhSquared, u1MulhSquaredSubx3, rMulu1MulhSquaredSubx3,
        s1MulhPowThree, y3, hMulz1, z3, NULL);

    return result;
}

Jacobian jacobian_multiply(mpz_t s, Jacobian jacobian, EllipticCurve ellipticCurve)
{
    if(!mpz_cmp_ui(s, 0))
    {
        return jacobian_infinity();
    }

    if(jacobian_isInfinity(jacobian))
    {
        return jacobian_infinity();
    }

    Jacobian jacobianN = jacobian_init(jacobian.x, jacobian.y, jacobian.z);
    Jacobian jacobianQ = jacobian_infinity();

    char *d = mpz_get_str(NULL, 2, s);

    for(int i = strlen(d) - 1; i >= 0; i--)
    {
        if(d[i] == '1')
        {
            Jacobian tmp = jacobian_add(jacobianQ, jacobianN, ellipticCurve);
            jacobian_destroy(jacobianQ);
            jacobianQ = tmp;
        }

        Jacobian tmp = jacobian_double(jacobianN, ellipticCurve);
        jacobian_destroy(jacobianN);
        jacobianN = tmp;
    }

    free(d);

    jacobian_destroy(jacobianN);

    return jacobianQ;
}
