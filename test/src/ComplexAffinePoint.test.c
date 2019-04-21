#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "complex/Complex.h"
#include "elliptic/ComplexAffinePoint.h"
#include "elliptic/EllipticCurve.h"


TEST multiplication_should_just_work(ComplexAffinePoint p, long s, ComplexAffinePoint expected)
{
    // Given
    mpz_t scalar;
    mpz_init_set_ui(scalar, s);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexAffinePoint result;
    int err = complexAffine_multiply(&result, scalar, p, ec);

    if (err)
    {
        ellipticCurve_destroy(ec);
        mpz_clear(scalar);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isEquals(result, expected));

    complexAffine_destroy(result);
    ellipticCurve_destroy(ec);
    mpz_clear(scalar);

    PASS();
}

SUITE(multiplication_suite)
{
    {
        ComplexAffinePoint p = complexAffine_initLong(0, 0, 1, 0);
        ComplexAffinePoint expected = complexAffine_initLong(0, 0, 4, 0);

        RUN_TESTp(multiplication_should_just_work, p, 2, expected);

        complexAffine_destroy(p);
        complexAffine_destroy(expected);
    }

    {
        ComplexAffinePoint p = complexAffine_initLong(0, 0, 4, 0);
        ComplexAffinePoint expected = complexAffine_initLong(0, 0, 1, 0);

        RUN_TESTp(multiplication_should_just_work, p, 2, expected);

        complexAffine_destroy(p);
        complexAffine_destroy(expected);
    }

    {
        ComplexAffinePoint p = complexAffine_initLong(2, 0, 2, 0);
        ComplexAffinePoint expected = complexAffine_infinity();

        RUN_TESTp(multiplication_should_just_work, p, 0, expected);

        complexAffine_destroy(p);
        complexAffine_destroy(expected);
    }
}

TEST adding_a_point_to_itself_with_y_equals_to_zero_should_yield_infinity(void)
{
    // Given
    ComplexAffinePoint p = complexAffine_initLong(1, 0, 0, 0);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexAffinePoint result;
    int err = complexAffine_add(&result, p, p, ec);

    if (err)
    {
        complexAffine_destroy(p);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isInfinity(result));

    complexAffine_destroy(p);
    complexAffine_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST adding_infinity_to_infinity_should_result_in_infinity(void)
{
    // Given
    ComplexAffinePoint infty = complexAffine_infinity();
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexAffinePoint result;
    int err = complexAffine_add(&result, infty, infty, ec);

    if (err)
    {
        complexAffine_destroy(infty);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isInfinity(result));

    complexAffine_destroy(infty);
    complexAffine_destroy(result);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST infinity_should_act_as_the_identity_element_for_addition(void)
{
    // Given
    ComplexAffinePoint p = complexAffine_initLong(0, 0, 1, 0);
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);
    ComplexAffinePoint infty = complexAffine_infinity();

    // When
    ComplexAffinePoint result1;
    int err = complexAffine_add(&result1, p, infty, ec);

    if (err)
    {
        complexAffine_destroy(p);
        complexAffine_destroy(infty);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    ComplexAffinePoint result2;
    err = complexAffine_add(&result2, infty, p, ec);

    if (err)
    {
        complexAffine_destroy(p);
        complexAffine_destroy(infty);
        complexAffine_destroy(result1);
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isEquals(p, result1));
    ASSERT(complexAffine_isEquals(p, result2));

    complexAffine_destroy(p);
    complexAffine_destroy(result1);
    complexAffine_destroy(result2);
    complexAffine_destroy(infty);
    ellipticCurve_destroy(ec);

    PASS();
}

TEST addition_on_non_special_cases_should_work_correctly(ComplexAffinePoint a, ComplexAffinePoint b, ComplexAffinePoint expected)
{
    // Given
    EllipticCurve ec = ellipticCurve_initLong(0, 1, 5);

    // When
    ComplexAffinePoint result;
    int err = complexAffine_add(&result, a, b, ec);

    if (err)
    {
        ellipticCurve_destroy(ec);

        FAIL();
    }

    // Then
    ASSERT(complexAffine_isEquals(result, expected));

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
        ComplexAffinePoint data[21] = {
            complexAffine_initLong(0, 0, 1, 0), complexAffine_initLong(0, 0, 1, 0), complexAffine_initLong(0, 0, 4, 0),
            complexAffine_initLong(0, 0, 4, 0), complexAffine_initLong(0, 0, 4, 0), complexAffine_initLong(0, 0, 1, 0),
            complexAffine_initLong(4, 0, 0, 0), complexAffine_initLong(0, 0, 4, 0), complexAffine_initLong(2, 0, 3, 0),
            complexAffine_initLong(0, 0, 4, 0), complexAffine_initLong(4, 0, 0, 0), complexAffine_initLong(2, 0, 3, 0),
            complexAffine_initLong(0, 0, 1, 0), complexAffine_initLong(0, 0, 4, 0), complexAffine_infinity(),
            complexAffine_initLong(2, 0, 2, 0), complexAffine_initLong(0, 0, 4, 0), complexAffine_initLong(4, 0, 0, 0),
            complexAffine_initLong(0, 0, 4, 0), complexAffine_initLong(2, 0, 2, 0), complexAffine_initLong(4, 0, 0, 0),
        };

        for (int i = 0; i < 7; ++i)
        {
            int offset = i * 3;
            int a = offset;
            int b = offset + 1;
            int expected = offset + 2;

            RUN_TESTp(addition_on_non_special_cases_should_work_correctly, data[a], data[b], data[expected]);

            complexAffine_destroy(data[a]);
            complexAffine_destroy(data[b]);
            complexAffine_destroy(data[expected]);
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
