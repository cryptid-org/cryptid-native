#include "elliptic/Divisor.h"
#include "elliptic/TatePairing.h"


Status tate_performPairing(Complex *result, int embeddingDegree, EllipticCurve ellipticCurve, mpz_t subgroupOrder, AffinePoint p, AffinePoint b)
{
    ComplexAffinePoint q;

    //start of the distortion map
    if(affine_isInfinity(b))
    {
        q = complexAffine_infinity();
    }
    else
    {
        mpz_t axi, bxi, three, one, addition, quotient, difference;
        mpz_inits(axi, bxi, three, one, addition, quotient, difference, NULL);
        Complex xi, tmp, xprime;

        mpz_sub_ui(difference, ellipticCurve.fieldOrder, 1);
        mpz_cdiv_q_ui(axi, difference, 2);

        mpz_set_ui(three, 3);
        mpz_add_ui(addition, ellipticCurve.fieldOrder, 1);
        mpz_cdiv_q_ui(quotient, addition, 4);
        mpz_powm(bxi, three, quotient, ellipticCurve.fieldOrder);

        mpz_set_ui(one, 1);
        tmp = complex_initMpz(one, bxi);
        xi = complex_modMulScalar(tmp, axi, ellipticCurve.fieldOrder);

        xprime = complex_modMulScalar(xi, b.x, ellipticCurve.fieldOrder);

        mpz_t zero;
        mpz_init_set_ui(zero, 0);
        Complex bY = complex_initMpz(b.y, zero);

        q = complexAffine_init(xprime, bY);

        complex_destroy(bY);
        mpz_clear(zero);

        mpz_clears(axi, bxi, three, one, addition, quotient, difference, NULL);
        complex_destroyMany(3, xi, tmp, xprime);
    }
    //end of the distortion map

    if(complexAffine_isInfinity(q))
    {
        complexAffine_destroy(q);
        *result = complex_initLong(1, 0);
        return SUCCESS;
    }

    //start of the miller algorithm
    Complex f, gVVQ, g2VMinus2VQ, g2VMinus2VQInv, frac, tmpF, gVPQ, gVPlusQ, gVPlusQInv;
    AffinePoint v, doubleV, vPlusP;

    f = complex_initLong(1, 0);
    v = affine_init(p.x, p.y);
    
    for(int i = mpz_sizeinbase(subgroupOrder, 2) - 2; i >= 0; --i)
    {
        //start of double step
        Status status = affine_add(&doubleV, v, v, ellipticCurve);
        if(status)
        {
            complexAffine_destroy(q);
            complex_destroy(f);
            affine_destroy(v);
            return status;
        }
        status = divisor_evaluateTangent(&gVVQ, ellipticCurve, v, q);
        if(status)
        {
            complexAffine_destroy(q);
            complex_destroy(f);
            affine_destroy(v);
            affine_destroy(doubleV);
            return status;
        }
        g2VMinus2VQ = divisor_evaluateVertical(ellipticCurve, doubleV, q);
        status = complex_multiplicativeInverse(&g2VMinus2VQInv, g2VMinus2VQ, ellipticCurve.fieldOrder);
        if(status)
        {
            complexAffine_destroy(q);
            affine_destroy(v);
            affine_destroy(doubleV);
            complex_destroyMany(3, f, gVVQ, g2VMinus2VQ);
            return status;
        }
        frac = complex_modMul(gVVQ, g2VMinus2VQInv, ellipticCurve.fieldOrder);
        tmpF = complex_modMul(f, f, ellipticCurve.fieldOrder);

        complex_destroy(f);
        f = complex_modMul(tmpF, frac, ellipticCurve.fieldOrder);

        affine_destroy(v);
        v = affine_init(doubleV.x, doubleV.y);

        complex_destroyMany(5, gVVQ, g2VMinus2VQ, g2VMinus2VQInv, frac, tmpF);
        affine_destroy(doubleV);
        //end of double step

        if(mpz_tstbit(subgroupOrder, i))
        {
            //start of add step
            status = affine_add(&vPlusP, v, p, ellipticCurve);
            if(status)
            {
                complexAffine_destroy(q);
                complex_destroy(f);
                affine_destroy(v);
                return status;
            }
            status = divisor_evaluateLine(&gVPQ, ellipticCurve, v, p, q);
            if(status)
            {
                complexAffine_destroy(q);
                complex_destroy(f);
                affine_destroy(v);
                affine_destroy(vPlusP);
                return status;
            }
            gVPlusQ = divisor_evaluateVertical(ellipticCurve, vPlusP, q);
            status = complex_multiplicativeInverse(&gVPlusQInv, gVPlusQ, ellipticCurve.fieldOrder);
            if(status)
            {
                complexAffine_destroy(q);
                affine_destroy(v);
                affine_destroy(vPlusP);
                complex_destroyMany(3, f, gVPQ, gVPlusQ);
                return status;
            }
            frac = complex_modMul(gVPQ, gVPlusQInv, ellipticCurve.fieldOrder);
            tmpF = complex_modMul(f, frac, ellipticCurve.fieldOrder);

            complex_destroy(f);
            f = complex_initMpz(tmpF.real, tmpF.imaginary);

            affine_destroy(v);
            v = affine_init(vPlusP.x, vPlusP.y);

            complex_destroyMany(5, gVPQ, gVPlusQ, gVPlusQInv, frac, tmpF);
            affine_destroy(vPlusP);
            //end of add step
        }
    }
    affine_destroy(v);
    complexAffine_destroy(q);
    //end of the miller algorithm

    //start of final exponentiation
    mpz_t exponent, pPow, exponentPart;
    mpz_inits(exponent, pPow, exponentPart, NULL);

    mpz_pow_ui(pPow, ellipticCurve.fieldOrder, embeddingDegree);
    mpz_sub_ui(exponentPart, pPow, 1);
    mpz_cdiv_q(exponent, exponentPart, subgroupOrder);

    *result = complex_modPow(f, exponent, ellipticCurve.fieldOrder);

    mpz_clears(exponent, pPow, exponentPart, NULL);
    complex_destroy(f);
    //end of final exponentiation

    return SUCCESS;
}
