#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"

TEST wnafmultiplication_should_just_work(const AffinePoint p, const long s, const AffinePoint expected)
{
    // Given
    mpz_t scalar;
    mpz_init_set_ui(scalar, s);
    EllipticCurve ec;
    ellipticCurve_initLong(&ec, 0, 1, 5);

    // When
    AffinePoint result;
    int err = affine_wNAFMultiply(&result, p, scalar, ec);

    if (err)
    {
        ellipticCurve_destroy(ec);
        mpz_clear(scalar);

        FAIL();
    }

    // Then
    ASSERT(affine_isEquals(result, expected));

    affine_destroy(result);
    ellipticCurve_destroy(ec);
    mpz_clear(scalar);

    PASS();
}

SUITE(wnafmultiplication_suite)
{
    {
        AffinePoint p;
        affine_initLong(&p, 0, 1);
        AffinePoint expected;
        affine_initLong(&expected, 0, 4);

        RUN_TESTp(wnafmultiplication_should_just_work, p, 2, expected);

        affine_destroy(p);
        affine_destroy(expected);
    }

    {
        AffinePoint p;
        affine_initLong(&p, 0, 4);
        AffinePoint expected;
        affine_initLong(&expected, 0, 1);

        RUN_TESTp(wnafmultiplication_should_just_work, p, 2, expected);

        affine_destroy(p);
        affine_destroy(expected);
    }

    {
        AffinePoint p;
        affine_initLong(&p, 2, 2);
        AffinePoint expected = affine_infinity();

        RUN_TESTp(wnafmultiplication_should_just_work, p, 0, expected);

        affine_destroy(p);
        affine_destroy(expected);
    }
}

TEST adding_a_point_to_itself_with_y_equals_to_zero_should_yield_infinity(void)
{
    // Given
    AffinePoint p;
    affine_initLong(&p, 1, 0);
    EllipticCurve ec;
    ellipticCurve_initLong(&ec, 0, 1, 5);

    // When
    AffinePoint result;
    int err = affine_add(&result, p, p, ec);

    if (err)
    {
        affine_destroy(p);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(affine_isInfinity(result));

    affine_destroy(p);
    affine_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST adding_infinity_to_infinity_should_result_in_infinity(void)
{
    // Given
    AffinePoint infty = affine_infinity();
    EllipticCurve ec;
    ellipticCurve_initLong(&ec, 0, 1, 5);

    // When
    AffinePoint result;
    int err = affine_add(&result, infty, infty, ec);

    if (err)
    {
        affine_destroy(infty);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(affine_isInfinity(result));

    affine_destroy(infty);
    affine_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST infinity_should_act_as_the_identity_element_for_addition(void)
{
    // Given
    AffinePoint p;
    affine_initLong(&p, 0, 1);
    EllipticCurve ec;
    ellipticCurve_initLong(&ec, 0, 1, 5);
    AffinePoint infty = affine_infinity();

    // When
    AffinePoint result1;
    int err = affine_add(&result1, p, infty, ec);

    if (err)
    {
        affine_destroy(p);
        affine_destroy(infty);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    AffinePoint result2;
    err = affine_add(&result2, infty, p, ec);

    if (err)
    {
        affine_destroy(p);
        affine_destroy(infty);
        affine_destroy(result1);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(affine_isEquals(p, result1));
    ASSERT(affine_isEquals(p, result2));

    affine_destroy(p);
    affine_destroy(result1);
    affine_destroy(result2);
    affine_destroy(infty);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST addition_on_non_special_cases_should_work_correctly(const AffinePoint a, const AffinePoint b, const AffinePoint expected)
{
    // Given
    EllipticCurve ec;
    ellipticCurve_initLong(&ec, 0, 1, 5);

    // When
    AffinePoint result;
    int err = affine_add(&result, a, b, ec);

    if (err)
    {
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(affine_isEquals(result, expected));

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
        AffinePoint data[21];
        
        affine_initLong(&(data[0]), 0, 1);
        affine_initLong(&(data[1]), 0, 1);
        affine_initLong(&(data[2]), 0, 4);
        affine_initLong(&(data[3]), 0, 4);
        affine_initLong(&(data[4]), 0, 4);
        affine_initLong(&(data[5]), 0, 1);
        affine_initLong(&(data[6]), 4, 0);
        affine_initLong(&(data[7]), 0, 4);
        affine_initLong(&(data[8]), 2, 3);
        affine_initLong(&(data[9]), 0, 4);
        affine_initLong(&(data[10]), 4, 0);
        affine_initLong(&(data[11]), 2, 3);
        affine_initLong(&(data[12]), 0, 1);
        affine_initLong(&(data[13]), 0, 4);
        data[14] = affine_infinity();
        affine_initLong(&(data[15]), 2, 2);
        affine_initLong(&(data[16]), 0, 4);
        affine_initLong(&(data[17]), 4, 0);
        affine_initLong(&(data[18]), 0, 4);
        affine_initLong(&(data[19]), 2, 2);
        affine_initLong(&(data[20]), 4, 0);

        for (int i = 0; i < 7; ++i)
        {
            int offset = i * 3;
            int a = offset;
            int b = offset + 1;
            int expected = offset + 2;
            RUN_TESTp(addition_on_non_special_cases_should_work_correctly, data[a], data[b], data[expected]);

            affine_destroy(data[a]);
            affine_destroy(data[b]);
            affine_destroy(data[expected]);
        }
    }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(wnafmultiplication_suite);
    RUN_SUITE(addition_suite);

    GREATEST_MAIN_END();
}
