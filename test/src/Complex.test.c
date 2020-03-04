#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "complex/Complex.h"


TEST complex_init_should_be_zero(void)
{
    // When
    Complex c;
    complex_init(&c);

    // Then
    ASSERT_EQ(mpz_cmp_ui(c.real, 0), 0);
    ASSERT_EQ(mpz_cmp_ui(c.imaginary, 0), 0);

    complex_destroy(c);

    PASS();
}

TEST complex_initLong_should_work(void)
{
    // When
    Complex c;
    complex_initLong(&c, 3, 4);

    // Then
    ASSERT_EQ(mpz_cmp_ui(c.real, 3), 0);
    ASSERT_EQ(mpz_cmp_ui(c.imaginary, 4), 0);

    complex_destroy(c);

    PASS();
}

TEST complex_initMpz_should_work(void)
{
    // Given
    mpz_t r, i;

    mpz_init_set_ui(r, 3);
    mpz_init_set_ui(i, 4);

    // When
    Complex c;
    complex_initMpz(&c, r, i);

    // Then
    ASSERT_EQ(mpz_cmp_ui(c.real, 3), 0);
    ASSERT_EQ(mpz_cmp_ui(c.imaginary, 4), 0);

    complex_destroy(c);
    mpz_clears(r, i, NULL);

    PASS();
}

SUITE(init_suite) 
{
    RUN_TEST(complex_init_should_be_zero);
    RUN_TEST(complex_initLong_should_work);
    RUN_TEST(complex_initMpz_should_work);
}

TEST zero_zero_should_act_as_the_additive_identity_element_for_any_p(void)
{
    // Given
    Complex zero;
    complex_init(&zero);

    Complex c;
    complex_initLong(&c, 4, 3);

    mpz_t p;
    mpz_init_set_ui(p, 5);

    // When
    Complex result1;
    complex_modAdd(&result1, c, zero, p);
    Complex result2;
    complex_modAdd(&result2, zero, c, p);

    // Then
    ASSERT(complex_isEquals(c, result1));
    ASSERT(complex_isEquals(c, result2));

    complex_destroyMany(4, zero, c, result1, result2);
    mpz_clear(p);

    PASS();
}

TEST modAdd_should_be_commutative(void)
{
    // Given
    Complex a;
    complex_initLong(&a, 4, 3);
    Complex b;
    complex_initLong(&b, 3, 1);
    Complex expected;
    complex_initLong(&expected, 2, 4);

    mpz_t p;
    mpz_init_set_ui(p, 5);

    // When
    Complex result1;
    complex_modAdd(&result1, a, b, p);
    Complex result2;
    complex_modAdd(&result2, b, a, p);

    // Then
    ASSERT(complex_isEquals(expected, result1));
    ASSERT(complex_isEquals(expected, result2));

    complex_destroyMany(5, a, b, result1, result2, expected);
    mpz_clear(p);

    PASS();
}

SUITE(add_suite)
{
    RUN_TEST(zero_zero_should_act_as_the_additive_identity_element_for_any_p);
    RUN_TEST(modAdd_should_be_commutative);
}

TEST additiveInverse_should_return_the_additive_inverse(const Complex complex, const Complex expected, const mpz_t p)
{
    // When
    Complex result;
    complex_additiveInverse(&result, complex, p);

    // Then
    ASSERT(complex_isEquals(result, expected));

    complex_destroy(result);

    PASS();
}

SUITE(additive_inverse_suite) 
{
    mpz_t p;
    mpz_init_set_ui(p, 5);

    {
        Complex c;
        complex_initLong(&c, 4, 3);
        Complex expected;
        complex_initLong(&expected, 1, 2);

        RUN_TESTp(additiveInverse_should_return_the_additive_inverse, c, expected, p);

        complex_destroyMany(2, c, expected);
    }

    {
        Complex c;
        complex_initLong(&c, 0, 0);
        Complex expected;
        complex_initLong(&expected, 0, 0);

        RUN_TESTp(additiveInverse_should_return_the_additive_inverse, c, expected, p);

        complex_destroyMany(2, c, expected);
    }

    mpz_clear(p);
}

TEST the_power_of_1_0_is_1_0_for_any_p(void)
{
    // Given
    Complex base;
    complex_initLong(&base, 1, 0);
    mpz_t exp, p;

    mpz_init_set_ui(exp, 5);
    mpz_init_set_ui(p, 5);

    // When
    Complex result;
    complex_modPow(&result, base, exp, p);

    // Then
    ASSERT(complex_isEquals(result, base));

    complex_destroyMany(2, result, base);
    mpz_clears(exp, p, NULL);

    PASS();
}

TEST the_modulo_power_of_complex_numbers_should_work_well(const Complex base, const mpz_t exp, const mpz_t p, const Complex expected)
{
    // When
    Complex result;
    complex_modPow(&result,base, exp, p);

    // Then
    ASSERT(complex_isEquals(result, expected));

    complex_destroy(result);

    PASS();
}

SUITE(modulo_power_suite)
{
    RUN_TEST(the_power_of_1_0_is_1_0_for_any_p);

    mpz_t p;
    mpz_init_set_ui(p, 7);
    
    {
        Complex base;
        complex_initLong(&base, 4, 1);
        Complex expected;
        complex_initLong(&expected, 3, 5);
        mpz_t exp;
        mpz_init_set_ui(exp, 3);

        RUN_TESTp(the_modulo_power_of_complex_numbers_should_work_well, base, exp, p, expected);

        complex_destroyMany(2, base, expected);
        mpz_clear(exp);
    }
    
    {
        Complex base;
        complex_initLong(&base, 0, 0);
        Complex expected;
        complex_initLong(&expected, 0, 0);
        mpz_t exp;
        mpz_init_set_ui(exp, 8);

        RUN_TESTp(the_modulo_power_of_complex_numbers_should_work_well, base, exp, p, expected);

        complex_destroyMany(2, base, expected);
        mpz_clear(exp);
    }

    {
        Complex base;
        complex_initLong(&base, 4, 3);
        Complex expected;
        complex_initLong(&expected, 6, 1);
        mpz_t exp;
        mpz_init_set_ui(exp, 5);

        RUN_TESTp(the_modulo_power_of_complex_numbers_should_work_well, base, exp, p, expected);

        complex_destroyMany(2, base, expected);
        mpz_clear(exp);
    }

    mpz_clear(p);
}

TEST GF_5_modMul_should_just_work(const long scalar, const long expectedReal, const long expectedImaginary)
{
    // Given
    mpz_t p, s;
    mpz_init_set_ui(p, 5);
    mpz_init_set_ui(s, scalar);

    Complex c;
    complex_initLong(&c, 2, 3);
    Complex expected;
    complex_initLong(&expected, expectedReal, expectedImaginary);

    // When
    Complex result;
    complex_modMulScalar(&result, c, s, p);

    // Then
    ASSERT(complex_isEquals(result, expected));

    complex_destroyMany(3, result, expected, c);
    mpz_clears(p, s, NULL);

    PASS();
}

SUITE(modulo_multiplication_with_scalar_suite)
{
    RUN_TESTp(GF_5_modMul_should_just_work, 0, 0, 0);
    RUN_TESTp(GF_5_modMul_should_just_work, 1, 2, 3);
    RUN_TESTp(GF_5_modMul_should_just_work, 2, 4, 1);
    RUN_TESTp(GF_5_modMul_should_just_work, 3, 1, 4);
    RUN_TESTp(GF_5_modMul_should_just_work, 4, 3, 2);
}

TEST the_multiplicative_inverse_of_1_0_should_be_1_0_for_any_p(void)
{
    // Given
    mpz_t p;
    mpz_init_set_ui(p, 5);

    Complex id;
    complex_initLong(&id, 1, 0);
    Complex result;

    // When
    CryptidStatus status = complex_multiplicativeInverse(&result, id, p);

    // Then
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    ASSERT(complex_isEquals(result, id));

    complex_destroyMany(2, result, id);
    mpz_clear(p);

    PASS();
}

TEST zero_zero_does_not_have_a_multiplicative_inverse(void)
{
    // Given
    mpz_t p;
    mpz_init_set_ui(p, 5);

    Complex zero;
    complex_initLong(&zero, 0, 0);
    Complex result;

    // When
    CryptidStatus status = complex_multiplicativeInverse(&result, zero, p);

    // Then
    ASSERT_EQ(status, CRYPTID_HAS_NO_MUL_INV_ERROR);

    complex_destroy(zero);
    mpz_clear(p);

    PASS();
}

TEST GF_7_multiplying_an_element_with_its_multiplicative_inverse_should_yield_the_identity_element(const long real, const long imaginary)
{
    // Given
    Complex id;
    complex_initLong(&id, 1, 0);
    mpz_t p;
    mpz_init_set_ui(p, 7);

    Complex c;
    complex_initLong(&c, real, imaginary);

    // When
    Complex inverse;
    CryptidStatus status = complex_multiplicativeInverse(&inverse, c, p);

    ASSERT_EQ(status, CRYPTID_SUCCESS);

    Complex result;
    complex_modMul(&result, c, inverse, p);

    // Then
    ASSERT(complex_isEquals(result, id));

    complex_destroyMany(4, result, inverse, c, id);
    mpz_clear(p);

    PASS();
}

SUITE(multiplicative_inverse_suite)
{
    RUN_TEST(the_multiplicative_inverse_of_1_0_should_be_1_0_for_any_p);
    RUN_TEST(zero_zero_does_not_have_a_multiplicative_inverse);

    {
        long p = 7;

        for (long n = 1; n < (p * p - 1); ++n)
        {
            long real = n / p;
            long imaginary = n % p;

            RUN_TESTp(GF_7_multiplying_an_element_with_its_multiplicative_inverse_should_yield_the_identity_element, real, imaginary);
        }
    }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(init_suite);
    RUN_SUITE(add_suite);
    RUN_SUITE(additive_inverse_suite);
    RUN_SUITE(modulo_power_suite);
    RUN_SUITE(modulo_multiplication_with_scalar_suite);
    RUN_SUITE(multiplicative_inverse_suite);

    GREATEST_MAIN_END();
}
