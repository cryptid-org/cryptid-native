#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "elliptic/Jacobian.h"


TEST multiplication_should_just_work(Jacobian p, long s, AffinePoint expected)
{
    // Given
    mpz_t scalar;
    mpz_init_set_ui(scalar, s);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    Jacobian jacobianResult = jacobian_multiply(scalar, p, ec);
    AffinePoint result;
    int err = jacobian_toAffine(&result, jacobianResult, ec);

    if (err)
    {
        printf("error\n");
        jacobian_destroy(jacobianResult);
        ellipticCurve_destroy(ec);
        mpz_clear(scalar);

        FAIL();
    }

    // Then
    ASSERT(affine_isEquals(result, expected));

    jacobian_destroy(jacobianResult);
    affine_destroy(result);
    ellipticCurve_destroy(ec);
    mpz_clear(scalar);

    PASS();
}

SUITE(multiplication_suite)
{
    {
        Jacobian p = jacobian_initLong(0, 1, 1);
        AffinePoint expected = affine_initLong(0, 4);

        RUN_TESTp(multiplication_should_just_work, p, 2, expected);

        jacobian_destroy(p);
        affine_destroy(expected);
    }

    {
        Jacobian p = jacobian_initLong(0, 4, 1);
        AffinePoint expected = affine_initLong(0, 1);

        RUN_TESTp(multiplication_should_just_work, p, 2, expected);

        jacobian_destroy(p);
        affine_destroy(expected);
    }

    {
        Jacobian p = jacobian_initLong(2, 2, 1);
        AffinePoint expected = affine_infinity();

        RUN_TESTp(multiplication_should_just_work, p, 0, expected);

        jacobian_destroy(p);
        affine_destroy(expected);
    }
}

TEST adding_a_point_to_itself_with_y_equals_to_zero_should_yield_infinity(void)
{
    // Given
    Jacobian p = jacobian_initLong(1, 0, 1);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    Jacobian result = jacobian_add(p, p, ec);

    // Then
    ASSERT(jacobian_isInfinity(result));

    jacobian_destroy(p);
    jacobian_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST adding_infinity_to_infinity_should_result_in_infinity(void)
{
    // Given
    Jacobian infty = jacobian_infinity();
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    Jacobian result = jacobian_add(infty, infty, ec);

    // Then
    ASSERT(jacobian_isInfinity(result));

    jacobian_destroy(infty);
    jacobian_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST infinity_should_act_as_the_identity_element_for_addition(void)
{
    // Given
    Jacobian p = jacobian_initLong(0, 1, 1);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);
    Jacobian infty = jacobian_infinity();

    // When
    Jacobian result1 = jacobian_add(p, infty, ec);

    Jacobian result2 = jacobian_add(infty, p, ec);

    // Then
    ASSERT(jacobian_isEquals(p, result1));
    ASSERT(jacobian_isEquals(p, result2));

    jacobian_destroy(p);
    jacobian_destroy(result1);
    jacobian_destroy(result2);
    jacobian_destroy(infty);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST addition_on_non_special_cases_should_work_correctly(Jacobian a, Jacobian b, AffinePoint expected)
{
    // Given
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    Jacobian jacobianResult = jacobian_add(a, b, ec);
    AffinePoint result;
    int err = jacobian_toAffine(&result, jacobianResult, ec);

    if (err)
    {
        jacobian_destroy(jacobianResult);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(affine_isEquals(result, expected));

    jacobian_destroy(jacobianResult);
    affine_destroy(result);
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
            Jacobian a;
            Jacobian b;
            AffinePoint expected;
        } data[7] = {
            {jacobian_initLong(0, 1, 1), jacobian_initLong(0, 1, 1), affine_initLong(0, 4)},
            {jacobian_initLong(0, 4, 1), jacobian_initLong(0, 4, 1), affine_initLong(0, 1)},
            {jacobian_initLong(4, 0, 1), jacobian_initLong(0, 4, 1), affine_initLong(2, 3)},
            {jacobian_initLong(0, 4, 1), jacobian_initLong(4, 0, 1), affine_initLong(2, 3)},
            {jacobian_initLong(0, 1, 1), jacobian_initLong(0, 4, 1), affine_infinity()},
            {jacobian_initLong(2, 2, 1), jacobian_initLong(0, 4, 1), affine_initLong(4, 0)},
            {jacobian_initLong(0, 4, 1), jacobian_initLong(2, 2, 1), affine_initLong(4, 0)}
        };

        for (int i = 0; i < 7; ++i)
        {
            RUN_TESTp(addition_on_non_special_cases_should_work_correctly, data[i].a, data[i].b, data[i].expected);

            jacobian_destroy(data[i].a);
            jacobian_destroy(data[i].b);
            affine_destroy(data[i].expected);
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
