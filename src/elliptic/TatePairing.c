#include "elliptic/Divisor.h"
#include "elliptic/TatePairing.h"


// References:
//   * [Intro-to-IBE] Luther Martin. 2008. Introduction to Identity-Based Encryption (Information Security and Privacy Series) (1 ed.). Artech House, Inc., Norwood, MA, USA. 


Status tate_performPairing(Complex *result, const int embeddingDegree, const EllipticCurve ellipticCurve,
                           const mpz_t subgroupOrder, const AffinePoint p, const AffinePoint b)
{
    // Implementation of Miller's algorithm as it's written on this page:
    // https://crypto.stanford.edu/pbc/notes/ep/miller.html
    ComplexAffinePoint q;

    // Distortion map - Creates linearly independent points
    // For examples on distortion maps, see [Intro-to-IBE p63.].
    //
    // Here we use a Xi distortion map, which involves calculating a \f$\xi\f$ value.
    // For Type-1 elliptic curves, this is calculated as follows:
    // \f$\xi = \frac{p - 1}{2}(1 + 3^{\frac{p + 1}{4}}i)\f$
    // where \f$p\f$ is the field order of the elliptic curve field.
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

        // \f$x^{\prime} = x \cdot xi\f$
        // \f$x \in \f$F_p\f$ | \f$xi\f$ \in \f$F_p^2\f$
        //
        // Here we assume, that we have to convert \f$x\f$ to \f$F_p^2\f$ and then perform the multiplication
        // according to the complex multiplication rules.
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
    
    if(complexAffine_isInfinity(q))
    {
        complexAffine_destroy(q);
        *result = complex_initLong(1, 0);
        return SUCCESS;
    }

    // Now p and q are linearly indenependent.
    // Here we start the actual Miller's algorithm.
    Complex f, gVVQ, g2VMinus2VQ, g2VMinus2VQInv, frac, tmpF, gVPQ, gVPlusQ, gVPlusQInv;
    AffinePoint v, doubleV, vPlusP;

    // 1. Set \f$f\f$ = 1 and \f$v\f$ = \f$p\f$
    f = complex_initLong(1, 0);
    v = affine_init(p.x, p.y);
    
    // 2. {@code for i = t - 1 to 0 do:}
    // where \f$t\f$ is the bitcount of the subgroup order.
    // Note, that we have to subtract 2 because of the allocation behavior
    // of GMP (please refer to https://gmplib.org/manual/Miscellaneous-Integer-Functions.html).
    for(int i = mpz_sizeinbase(subgroupOrder, 2) - 2; i >= 0; --i)
    {
        // Double step
        // \f$f = f^{2} \frac{g_{v, v}(q)}{g_{2v, -2v}(q)}\f$
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
        // \f$v = 2v\f$
        v = affine_init(doubleV.x, doubleV.y);

        complex_destroyMany(5, gVVQ, g2VMinus2VQ, g2VMinus2VQInv, frac, tmpF);
        affine_destroy(doubleV);
        
        if(mpz_tstbit(subgroupOrder, i))
        {
            // Add step
            // \f$f = f \frac{g_{v, p}(q)}{g_{v + p, -(b + p)}(q)}\f$
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
            // \f$v = v + p\f$
            v = affine_init(vPlusP.x, vPlusP.y);

            complex_destroyMany(5, gVPQ, gVPlusQ, gVPlusQInv, frac, tmpF);
            affine_destroy(vPlusP);
        }
    }
    affine_destroy(v);
    complexAffine_destroy(q);

    // Final Exponentiation
    mpz_t exponent, pPow, exponentPart;
    mpz_inits(exponent, pPow, exponentPart, NULL);

    mpz_pow_ui(pPow, ellipticCurve.fieldOrder, embeddingDegree);
    mpz_sub_ui(exponentPart, pPow, 1);
    mpz_cdiv_q(exponent, exponentPart, subgroupOrder);

    *result = complex_modPow(f, exponent, ellipticCurve.fieldOrder);

    mpz_clears(exponent, pPow, exponentPart, NULL);
    complex_destroy(f);

    return SUCCESS;
}
