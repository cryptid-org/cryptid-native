#include <stdlib.h>
#include <string.h>

#include "elliptic/AffinePoint.h"


AffinePoint affine_init(mpz_t x, mpz_t y)
{
    AffinePoint affinePoint;

    mpz_inits(affinePoint.x, affinePoint.y, NULL);
    mpz_set(affinePoint.x, x);
    mpz_set(affinePoint.y, y);

    return affinePoint;
}

AffinePoint affine_initLong(long x, long y)
{
    AffinePoint affinePoint;
    
    mpz_inits(affinePoint.x, affinePoint.y, NULL);
    mpz_set_si(affinePoint.x, x);
    mpz_set_si(affinePoint.y, y);

    return affinePoint;
}

void affine_destroy(AffinePoint affinePoint)
{
    mpz_clears(affinePoint.x, affinePoint.y, NULL);
}

AffinePoint affine_infinity()
{
    return affine_initLong(-1, -1);
}

int affine_isEquals(AffinePoint affinePoint1, AffinePoint affinePoint2)
{
    if(!mpz_cmp(affinePoint1.x, affinePoint2.x) && !mpz_cmp(affinePoint1.y, affinePoint2.y))
    {
        return 1;
    }

    return 0;
}

int affine_isInfinity(AffinePoint affinePoint)
{
    AffinePoint infty = affine_infinity();

    int result = affine_isEquals(infty, affinePoint);
    affine_destroy(infty);

    return result;
}

Status affine_double(AffinePoint *result, AffinePoint affinePoint, EllipticCurve ellipticCurve)
{
    if(affine_isInfinity(affinePoint))
    {
        *result = affine_infinity();
        return SUCCESS;
    }

    mpz_t x1PowTwo, threex1PowTwo, num, y1MulTwo, denom, numMulDenom, m, mPowTwo, mPowTwoSubx1, x3, x1Subx3, mMulx1Subx3, y3;

    if(!mpz_cmp_ui(affinePoint.y, 0))
    {
        *result = affine_infinity();
        return SUCCESS;
    }

    mpz_inits(x1PowTwo, threex1PowTwo, num, y1MulTwo, denom, numMulDenom, m, mPowTwo, mPowTwoSubx1, x3, x1Subx3, mMulx1Subx3, y3, NULL);

    mpz_powm_ui(x1PowTwo, affinePoint.x, 2, ellipticCurve.fieldOrder);
    mpz_mul_ui(threex1PowTwo, x1PowTwo, 3);
    mpz_add(num, threex1PowTwo, ellipticCurve.a);

    mpz_mul_ui(y1MulTwo, affinePoint.y, 2);
    mpz_invert(denom, y1MulTwo, ellipticCurve.fieldOrder);

    mpz_mul(numMulDenom, num, denom);
    mpz_mod(m, numMulDenom, ellipticCurve.fieldOrder);

    mpz_powm_ui(mPowTwo, m, 2, ellipticCurve.fieldOrder);
    mpz_sub(mPowTwoSubx1, mPowTwo, affinePoint.x);
    mpz_mod(mPowTwoSubx1, mPowTwoSubx1, ellipticCurve.fieldOrder);
    mpz_sub(x3, mPowTwoSubx1, affinePoint.x);
    mpz_mod(x3, x3, ellipticCurve.fieldOrder);

    mpz_sub(x1Subx3, affinePoint.x, x3);
    mpz_mod(x1Subx3, x1Subx3, ellipticCurve.fieldOrder);
    mpz_mul(mMulx1Subx3, m, x1Subx3);
    mpz_sub(y3, mMulx1Subx3, affinePoint.y);
    mpz_mod(y3, y3, ellipticCurve.fieldOrder);

    *result = affine_init(x3, y3);

    mpz_clears(x1PowTwo, threex1PowTwo, num, y1MulTwo, denom, numMulDenom, m, mPowTwo, mPowTwoSubx1, x3, x1Subx3, mMulx1Subx3, y3, NULL);

    return SUCCESS;
}

Status affine_add(AffinePoint *result, AffinePoint affinePoint1, AffinePoint affinePoint2, EllipticCurve ellipticCurve)
{
    mpz_t m, num, denom, numMulDenom, y2Suby1, x2Subx1, x2Subx1Mod, x3, y3, mPowTwo, mPowTwoSubx1, x1Subx3, mMulx1Subx3;

    if(affine_isInfinity(affinePoint1))
    {
        *result = affine_init(affinePoint2.x, affinePoint2.y);
        return SUCCESS;
    }

    if(affine_isInfinity(affinePoint2))
    {
        *result = affine_init(affinePoint1.x, affinePoint1.y);
        return SUCCESS;
    }

    mpz_inits(m, num, denom, numMulDenom, y2Suby1, x2Subx1, x2Subx1Mod, x3, y3, mPowTwo, mPowTwoSubx1, x1Subx3, mMulx1Subx3, NULL);

    if(affine_isEquals(affinePoint1, affinePoint2))
    {
        mpz_clears(m, num, denom, numMulDenom, y2Suby1, x2Subx1, x2Subx1Mod, x3, y3, mPowTwo, mPowTwoSubx1, x1Subx3, mMulx1Subx3, NULL);
        Status status = affine_double(result, affinePoint1, ellipticCurve);
        return status;
    }
    else
    {
        if(!mpz_cmp(affinePoint1.x, affinePoint2.x))
        {
            mpz_clears(m, num, denom, numMulDenom, y2Suby1, x2Subx1, x2Subx1Mod, x3, y3, mPowTwo, mPowTwoSubx1, x1Subx3, mMulx1Subx3, NULL);
            *result = affine_infinity();
            return SUCCESS;
        }

        mpz_sub(y2Suby1, affinePoint2.y, affinePoint1.y);
        mpz_mod(num, y2Suby1, ellipticCurve.fieldOrder);

        mpz_sub(x2Subx1, affinePoint2.x, affinePoint1.x);
        mpz_mod(x2Subx1Mod, x2Subx1, ellipticCurve.fieldOrder);
        mpz_invert(denom, x2Subx1Mod, ellipticCurve.fieldOrder);

        mpz_mul(numMulDenom, num, denom);
        mpz_mod(m, numMulDenom, ellipticCurve.fieldOrder);
    }

    mpz_powm_ui(mPowTwo, m, 2, ellipticCurve.fieldOrder);
    mpz_sub(mPowTwoSubx1, mPowTwo, affinePoint1.x);
    mpz_sub(x3, mPowTwoSubx1, affinePoint2.x);
    mpz_mod(x3, x3, ellipticCurve.fieldOrder);

    mpz_sub(x1Subx3, affinePoint1.x, x3);
    mpz_mul(mMulx1Subx3, m, x1Subx3);
    mpz_sub(y3, mMulx1Subx3, affinePoint1.y);
    mpz_mod(y3, y3, ellipticCurve.fieldOrder);

    *result = affine_init(x3, y3);

    mpz_clears(m, num, denom, numMulDenom, y2Suby1, x2Subx1, x2Subx1Mod, x3, y3, mPowTwo, mPowTwoSubx1, x1Subx3, mMulx1Subx3, NULL);

    return SUCCESS;
}

Status affine_multiply(AffinePoint *result, mpz_t s, AffinePoint affinePoint, EllipticCurve ellipticCurve)
{
    if(!mpz_cmp_ui(s, 0))
    {
        *result = affine_infinity();
        return SUCCESS;
    }

    if(affine_isInfinity(affinePoint))
    {
        *result = affine_infinity();
        return SUCCESS;
    }

    AffinePoint pointN = affine_init(affinePoint.x, affinePoint.y);
    AffinePoint pointQ = affine_infinity();

    char *d = mpz_get_str(NULL, 2, s);

    for(int i = strlen(d) - 1; i >= 0; i--)
    {
        Status status;

        if(d[i] == '1')
        {
            AffinePoint tmp;
            status = affine_add(&tmp, pointQ, pointN, ellipticCurve);
            if(status)
            {
                affine_destroy(pointN);
                affine_destroy(pointQ);
                free(d);
                return status;
            }
            affine_destroy(pointQ);
            pointQ = tmp;
        }

        AffinePoint tmp;
        status = affine_double(&tmp, pointN, ellipticCurve);
        if(status)
        {
            affine_destroy(pointN);
            affine_destroy(pointQ);
            free(d);
            return status;
        }
        affine_destroy(pointN);
        pointN = tmp;
    }

    free(d);

    affine_destroy(pointN);

    *result = pointQ;
    return SUCCESS;
}

// Status-t javitani, memory leakeket csekkolni
Status affine_wNAFMultiply(AffinePoint *result, mpz_t s, AffinePoint affinePoint, EllipticCurve ellipticCurve) {
    mpz_t d;
    mpz_init_set(d, s);

    int w = 5;
    int twoPowW = 32;
    int twoPowWSubOne = 16;
    AffinePoint preCalculatedPoints[16];
    int* nafForm = (int*)calloc(0, sizeof(int));

    mpz_t yNegate, yNegateModP, tmpS;
    mpz_inits(yNegate, yNegateModP, NULL);
    mpz_neg(yNegate, affinePoint.y);
    mpz_mod(yNegateModP, yNegate, ellipticCurve.fieldOrder);

    preCalculatedPoints[0] = affine_init(affinePoint.x, yNegateModP);
    preCalculatedPoints[1] = affine_init(affinePoint.x, affinePoint.y);
    mpz_clears(yNegate, yNegateModP, NULL);

    int actualIndex = 2;

    Status status;

    for(int i = 3; i < twoPowWSubOne; i += 2)
    {
        mpz_init_set_ui(tmpS, i);

        status = affine_multiply(&preCalculatedPoints[actualIndex + 1], tmpS, affinePoint, ellipticCurve);
        if(status)
        {
            mpz_clears(d, tmpS, NULL);
            for(int j = 0; j < actualIndex; j++)
            {
                affine_destroy(preCalculatedPoints[j]);
            }
            return status;
        }

        mpz_inits(yNegate, yNegateModP, NULL);

        mpz_neg(yNegate, preCalculatedPoints[actualIndex + 1].y);
        mpz_mod(yNegateModP, yNegate, ellipticCurve.fieldOrder);

        preCalculatedPoints[actualIndex] = affine_init(preCalculatedPoints[actualIndex + 1].x, yNegateModP);

        actualIndex += 2;

        mpz_clears(yNegate, yNegateModP, tmpS, NULL);
    }

    mpz_t dModTwo, mod, dSub, dDivideTwo;

    int i = 0;
    while(mpz_cmp_ui(d, 0) > 0)
    {
        nafForm = (int*)realloc(nafForm, (i + 1) * sizeof(int));
        mpz_init(dModTwo);
        mpz_mod_ui(dModTwo, d, 2);
        if(mpz_cmp_ui(dModTwo, 1) == 0)
        {
            mpz_init(mod);
            mpz_mod_ui(mod, d, twoPowW);
            mpz_init(dSub);
            if(mpz_cmp_ui(mod, twoPowWSubOne) >= 0)
            {
                nafForm[i] = mpz_get_ui(mod) - twoPowW;
                mpz_add_ui(dSub, d, abs(nafForm[i]));
            }
            else
            {
                nafForm[i] = mpz_get_ui(mod);
                mpz_sub_ui(dSub, d, nafForm[i]);
            }
            mpz_clear(d);
            mpz_init_set(d, dSub);
            mpz_clears(dSub, mod, NULL);
        }
        else
        {
            nafForm[i] = 0;
        }
        mpz_init(dDivideTwo);
        mpz_divexact_ui(dDivideTwo, d, 2);
        mpz_clear(d);
        mpz_init_set(d, dDivideTwo);
        mpz_clears(dDivideTwo, dModTwo, NULL);
        i++;
    }
    mpz_clear(d);

    AffinePoint pointQ = affine_infinity();

    for(int j = i - 1; j >= 0; j--)
    {
        AffinePoint tmp;
        status = affine_double(&tmp, pointQ, ellipticCurve);
        if(status)
        {
            for(int o = 0; o < 16; o++)
            {
                affine_destroy(preCalculatedPoints[o]);
            }
            affine_destroy(pointQ);
            return status;
        }
        affine_destroy(pointQ);
        pointQ = affine_init(tmp.x, tmp.y);
        affine_destroy(tmp);
        int chosen = nafForm[j];
        if(chosen != 0)
        {
            int index = chosen>0?chosen:abs(chosen) - 1;
            AffinePoint tmp2;
            status = affine_add(&tmp2, pointQ, preCalculatedPoints[index], ellipticCurve);
            if(status)
            {
                for(int o = 0; o < 16; o++)
                {
                    affine_destroy(preCalculatedPoints[o]);
                }
                affine_destroy(pointQ);
                return status;
            }
            affine_destroy(pointQ);
            pointQ = affine_init(tmp2.x, tmp2.y);
            affine_destroy(tmp2);
        }
    }

    for(int o = 0; o < 16; o++)
    {
        affine_destroy(preCalculatedPoints[o]);
    }
    free(nafForm);
    *result = pointQ;
    return SUCCESS;
}

int affine_isOnCurve(AffinePoint point, EllipticCurve ellipticCurve)
{
    mpz_t ySquared, xCubed, ax, xCubedPlusAx, rhs;
    mpz_inits(ySquared, xCubed, ax, xCubedPlusAx, rhs, NULL);

    mpz_powm_ui(ySquared, point.y, 2, ellipticCurve.fieldOrder);
    mpz_powm_ui(xCubed, point.x, 3, ellipticCurve.fieldOrder);

    mpz_mul(ax, point.x, ellipticCurve.a);
    mpz_mod(ax, ax, ellipticCurve.fieldOrder);

    mpz_add(xCubedPlusAx, xCubed, ax);
    mpz_mod(xCubedPlusAx, xCubedPlusAx, ellipticCurve.fieldOrder);

    mpz_add(rhs, xCubedPlusAx, ellipticCurve.b);
    mpz_mod(rhs, rhs, ellipticCurve.fieldOrder);

    int result = !mpz_cmp(ySquared, rhs);

    mpz_clears(ySquared, xCubed, ax, xCubedPlusAx, rhs, NULL);

    return result;
}
