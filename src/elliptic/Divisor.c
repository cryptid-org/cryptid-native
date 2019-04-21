#include "gmp.h"

#include "elliptic/Divisor.h"


Complex divisor_evaluateVertical(EllipticCurve ec, AffinePoint a, ComplexAffinePoint b)
{
    Complex result;

    if(affine_isInfinity(a))
    {
        result = complex_initLong(1, 0);
        return result;
    }

    mpz_t axAddInv;
    mpz_init(axAddInv);
    mpz_neg(axAddInv, a.x);
    mpz_mod(axAddInv, axAddInv, ec.fieldOrder);

    result = complex_modAddScalar(b.x, axAddInv, ec.fieldOrder);

    mpz_clear(axAddInv);

    return result;
}

Status divisor_evaluateTangent(Complex* result, EllipticCurve ec, AffinePoint a, ComplexAffinePoint b)
{
    if(complexAffine_isInfinity(b))
    {
        return DIVISOR_OF_TANGENT_INFINITY_ERROR;
    }

    if(affine_isInfinity(a))
    {
        *result = complex_initLong(1, 0);
        return SUCCESS;
    }

    if(!mpz_cmp_ui(a.y, 0))
    {
        *result = divisor_evaluateVertical(ec, a, b);
        return SUCCESS;
    }

    Complex axB, byB, resultPart;
    mpz_t threeAddInv, minusThree, xasquared, aprime, bprime, bAddInv, bAddInvyA, axA, axAaddInv, c;
    mpz_inits(threeAddInv, minusThree, xasquared, aprime, bprime, bAddInv, bAddInvyA, axA, axAaddInv, c, NULL);

    mpz_set_si(minusThree, -3);
    mpz_mod(threeAddInv, minusThree, ec.fieldOrder);
    mpz_powm_ui(xasquared, a.x, 2, ec.fieldOrder);
    mpz_mul(aprime, xasquared, threeAddInv);
    mpz_mod(aprime, aprime, ec.fieldOrder);

    mpz_mul_ui(bprime, a.y, 2);
    mpz_mod(bprime, bprime, ec.fieldOrder);

    mpz_neg(bAddInv, bprime);
    mpz_mod(bAddInv, bAddInv, ec.fieldOrder);
    mpz_mul(bAddInvyA, bAddInv, a.y);
    mpz_mod(bAddInvyA, bAddInvyA, ec.fieldOrder);
    mpz_mul(axA, aprime, a.x);
    mpz_mod(axA, axA, ec.fieldOrder);
    mpz_neg(axAaddInv, axA);
    mpz_mod(axAaddInv, axAaddInv, ec.fieldOrder);
    mpz_add(c, bAddInvyA, axAaddInv);
    mpz_mod(c, c, ec.fieldOrder);

    axB = complex_modMulScalar(b.x, aprime, ec.fieldOrder);
    byB = complex_modMulScalar(b.y, bprime, ec.fieldOrder);
    resultPart = complex_modAdd(axB, byB, ec.fieldOrder);
    *result = complex_modAddScalar(resultPart, c, ec.fieldOrder);

    complex_destroyMany(3, axB, byB, resultPart);
    mpz_clears(threeAddInv, minusThree, xasquared, aprime, bprime, bAddInv, bAddInvyA, axA, axAaddInv, c, NULL);
    return SUCCESS;
}

Status divisor_evaluateLine(Complex* result, EllipticCurve ec, AffinePoint a, AffinePoint aprime, ComplexAffinePoint b)
{
    if(complexAffine_isInfinity(b))
    {
        return DIVISOR_OF_LINE_INFINITY_ERROR;
    }

    if(affine_isInfinity(a))
    {
        *result = divisor_evaluateVertical(ec, aprime, b);
        return SUCCESS;
    }

    AffinePoint aPlusAPrime;
    Status status = affine_add(&aPlusAPrime, a, aprime, ec);
    if(status)
    {
        return status;
    }

    if(affine_isInfinity(aprime) || affine_isInfinity(aPlusAPrime))
    {
        *result = divisor_evaluateVertical(ec, a, b);
        affine_destroy(aPlusAPrime);
        return SUCCESS;
    }
    affine_destroy(aPlusAPrime);

    if(affine_isEquals(a, aprime))
    {
        return divisor_evaluateTangent(result, ec, a, b);
    }

    mpz_t linea, lineb, linebaddinv, q, t, taddinv, linec;
    mpz_inits(linea, lineb, linebaddinv, q, t, taddinv, linec, NULL);
    Complex axb, byb, resultPart;

    mpz_sub(linea, a.y, aprime.y);
    mpz_mod(linea, linea, ec.fieldOrder);

    mpz_sub(lineb, aprime.x, a.x);
    mpz_mod(lineb, lineb, ec.fieldOrder);

    mpz_neg(linebaddinv, lineb);
    mpz_mod(linebaddinv, linebaddinv, ec.fieldOrder);
    mpz_mul(q, linebaddinv, a.y);
    mpz_mod(q, q, ec.fieldOrder);
    mpz_mul(t, linea, a.x);
    mpz_mod(t, t, ec.fieldOrder);
    mpz_neg(taddinv, t);
    mpz_mod(taddinv, taddinv, ec.fieldOrder);
    mpz_add(linec, q, taddinv);
    mpz_mod(linec, linec, ec.fieldOrder);

    axb = complex_modMulScalar(b.x, linea, ec.fieldOrder);
    byb = complex_modMulScalar(b.y, lineb, ec.fieldOrder);
    resultPart = complex_modAddScalar(byb, linec, ec.fieldOrder);
    *result = complex_modAdd(axb, resultPart, ec.fieldOrder);

    mpz_clears(linea, lineb, linebaddinv, q, t, taddinv, linec, NULL);
    complex_destroyMany(3, axb, byb, resultPart);

    return SUCCESS;
}