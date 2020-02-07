#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "elliptic/TatePairing.h"


TEST GF_131_modified_tate_pairing_should_just_work(const long n, const Complex expected)
{
    // Given
    int embeddingDegree = 2;
    mpz_t subgroupOrder, mul;
    mpz_init_set_ui(subgroupOrder, 11);
    mpz_init_set_ui(mul, n);
    EllipticCurve ec;
    ellipticCurve_initLong(&ec, 0, 1, 131);
    AffinePoint a;
    affine_initLong(&a, 98, 58);
    AffinePoint b;
    affine_multiply(&b, mul, a, ec);

    // When
    Complex result;
    CryptidStatus status = tate_performPairing(&result, embeddingDegree, ec, subgroupOrder, a, b);

    // Then
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    ASSERT(complex_isEquals(result, expected));

    affine_destroy(a);
    affine_destroy(b);
    mpz_clears(subgroupOrder, mul, NULL);
    ellipticCurve_destroy(ec);
    complex_destroy(result);

    PASS();
}

TEST RFC_5091_tate_pairing_should_work(void)
{
    // Given
    int embeddingDegree = 2;
    mpz_t subgroupOrder, p, zero, ax, ay, bx, by, expecteda, expectedb;
    mpz_inits(subgroupOrder, p, ax, ay, bx, by, expecteda, expectedb, NULL);
    
    mpz_init_set_ui(zero, 0);

    mpz_set_str(subgroupOrder, "fffffffffffffffffffffffffffbffff", 16);
    mpz_set_str(p, "bffffffffffffffffffffffffffcffff3", 16);

    mpz_set_str(ax, "489a03c58dcf7fcfc97e99ffef0bb4634", 16);
    mpz_set_str(ay, "510c6972d795ec0c2b081b81de767f808", 16);
    mpz_set_str(bx, "40e98b9382e0b1fa6747dcb1655f54f75", 16);
    mpz_set_str(by, "b497a6a02e7611511d0db2ff133b32a3f", 16);
    AffinePoint a;
    affine_init(&a, ax, ay);
    AffinePoint b;
    affine_init(&b, bx, by);

    mpz_set_str(expecteda, "8b2cac13cbd422658f9e5757b85493818", 16);
    mpz_set_str(expectedb, "bc6af59f54d0a5d83c8efd8f5214fad3c", 16);
    Complex expected;
    complex_initMpz(&expected, expecteda, expectedb);

    EllipticCurve ec;
    {
        mpz_t one;
        mpz_init_set_ui(one, 1);
        ellipticCurve_init(&ec, zero, one, p);
        mpz_clear(one);
    }

    // When
    Complex result;
    CryptidStatus status = tate_performPairing(&result, embeddingDegree, ec, subgroupOrder, a, b);

    // Then
    ASSERT_EQ(status, CRYPTID_SUCCESS);
    ASSERT(complex_isEquals(result, expected));

    affine_destroy(a);
    affine_destroy(b);
    complex_destroyMany(2, result, expected);
    ellipticCurve_destroy(ec);
    mpz_clears(subgroupOrder, p, zero, ax, ay, bx, by, expecteda, expectedb, NULL);
    
    PASS();
}

SUITE(tate_pairing_suite)
{
    {
        Complex expected[11];
        
        complex_initLong(&(expected[0]), 28, 93);
        complex_initLong(&(expected[1]), 126, 99);
        complex_initLong(&(expected[2]), 85, 80);
        complex_initLong(&(expected[3]), 49, 58);
        complex_initLong(&(expected[4]), 39, 24);
        complex_initLong(&(expected[5]), 39, 107);
        complex_initLong(&(expected[6]), 49, 73);
        complex_initLong(&(expected[7]), 85, 51);
        complex_initLong(&(expected[8]), 126, 32);
        complex_initLong(&(expected[9]), 28, 38);
        complex_initLong(&(expected[10]), 1, 0);

        for (long n = 1; n <= 11; ++n)
        {
            RUN_TESTp(GF_131_modified_tate_pairing_should_just_work, n, expected[n - 1]);

            complex_destroy(expected[n - 1]);
        }
    }

    RUN_TEST(RFC_5091_tate_pairing_should_work);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(tate_pairing_suite);

    GREATEST_MAIN_END();
}
