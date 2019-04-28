#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

#include "complex/Complex.h"
#include "elliptic/ComplexJacobian.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"


TEST multiplication_should_just_work(ComplexJacobian p, long s, ComplexAffinePoint expected)
{
    // Given
    mpz_t scalar;
    mpz_init_set_ui(scalar, s);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexJacobian jacobianResult = complexJacobian_multiply(scalar, p, ec);
    ComplexAffinePoint result;
    int err = complexJacobian_toComplexAffine(&result, jacobianResult, ec);

    if (err)
    {
        printf("error\n");
        complexJacobian_destroy(jacobianResult);
        ellipticCurve_destroy(ec);
        mpz_clear(scalar);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isEquals(result, expected));

    complexJacobian_destroy(jacobianResult);
    complexAffine_destroy(result);
    ellipticCurve_destroy(ec);
    mpz_clear(scalar);

    PASS();
}

SUITE(multiplication_suite)
{
    {
        ComplexJacobian p = complexJacobian_initLong(0, 0, 1, 0, 1, 0);
        ComplexAffinePoint expected = complexAffine_initLong(0, 0, 4, 0);

        RUN_TESTp(multiplication_should_just_work, p, 2, expected);

        complexJacobian_destroy(p);
        complexAffine_destroy(expected);
    }

    {
        ComplexJacobian p = complexJacobian_initLong(0, 0, 4, 0, 1, 0);
        ComplexAffinePoint expected = complexAffine_initLong(0, 0, 1, 0);

        RUN_TESTp(multiplication_should_just_work, p, 2, expected);

        complexJacobian_destroy(p);
        complexAffine_destroy(expected);
    }

    {
        ComplexJacobian p = complexJacobian_initLong(2, 0, 2, 0, 1, 0);
        ComplexAffinePoint expected = complexAffine_infinity();

        RUN_TESTp(multiplication_should_just_work, p, 0, expected);

        complexJacobian_destroy(p);
        complexAffine_destroy(expected);
    }
}

TEST adding_a_point_to_itself_with_y_equals_to_zero_should_yield_infinity(void)
{
    // Given
    ComplexJacobian p = complexJacobian_initLong(1, 0, 0, 0, 1, 0);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexJacobian result = complexJacobian_add(p, p, ec);

    // Then
    ASSERT(complexJacobian_isInfinity(result));

    complexJacobian_destroy(p);
    complexJacobian_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST adding_infinity_to_infinity_should_result_in_infinity(void)
{
    // Given
    ComplexJacobian infty = complexJacobian_infinity();
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexJacobian result = complexJacobian_add(infty, infty, ec);

    // Then
    ASSERT(complexJacobian_isInfinity(result));

    complexJacobian_destroy(infty);
    complexJacobian_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST infinity_should_act_as_the_identity_element_for_addition(void)
{
    // Given
    ComplexJacobian p = complexJacobian_initLong(0, 0, 1, 0, 1, 0);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);
    ComplexJacobian infty = complexJacobian_infinity();

    // When
    ComplexJacobian result1 = complexJacobian_add(p, infty, ec);

    ComplexJacobian result2 = complexJacobian_add(infty, p, ec);

    // Then
    ASSERT(complexJacobian_isEquals(p, result1));
    ASSERT(complexJacobian_isEquals(p, result2));

    complexJacobian_destroy(p);
    complexJacobian_destroy(result1);
    complexJacobian_destroy(result2);
    complexJacobian_destroy(infty);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST addition_on_non_special_cases_should_work_correctly(ComplexJacobian a, ComplexJacobian b, ComplexAffinePoint expected)
{
    // Given
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexJacobian jacobianResult = complexJacobian_add(a, b, ec);
    ComplexAffinePoint result;
    int err = complexJacobian_toComplexAffine(&result, jacobianResult, ec);

    if (err)
    {
        complexJacobian_destroy(jacobianResult);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isEquals(result, expected));

    complexJacobian_destroy(jacobianResult);
    complexAffine_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

SUITE(addition_suite) 
{
    RUN_TEST(adding_a_point_to_itself_with_y_equals_to_zero_should_yield_infinity);
    RUN_TEST(adding_infinity_to_infinity_should_result_in_infinity);
    RUN_TEST(infinity_should_act_as_the_identity_element_for_addition);

    {
        struct DataStruct
        {
            ComplexJacobian a;
            ComplexJacobian b;
            ComplexAffinePoint expected;
        } data[7] = {
            {complexJacobian_initLong(0, 0, 1, 0, 1, 0), complexJacobian_initLong(0, 0, 1, 0, 1, 0), complexAffine_initLong(0, 0, 4, 0)},
            {complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexAffine_initLong(0, 0, 1, 0)},
            {complexJacobian_initLong(4, 0, 0, 0, 1, 0), complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexAffine_initLong(2, 0, 3, 0)},
            {complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexJacobian_initLong(4, 0, 0, 0, 1, 0), complexAffine_initLong(2, 0, 3, 0)},
            {complexJacobian_initLong(0, 0, 1, 0, 1, 0), complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexAffine_infinity()},
            {complexJacobian_initLong(2, 0, 2, 0, 1, 0), complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexAffine_initLong(4, 0, 0, 0)},
            {complexJacobian_initLong(0, 0, 4, 0, 1, 0), complexJacobian_initLong(2, 0, 2, 0, 1, 0), complexAffine_initLong(4, 0, 0, 0)}
        };

        for (int i = 0; i < 7; ++i)
        {
            RUN_TESTp(addition_on_non_special_cases_should_work_correctly, data[i].a, data[i].b, data[i].expected);

            complexJacobian_destroy(data[i].a);
            complexJacobian_destroy(data[i].b);
            complexAffine_destroy(data[i].expected);
        }
    }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(multiplication_suite);
    RUN_SUITE(addition_suite);

    GREATEST_MAIN_END();
}
