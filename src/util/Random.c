#include "util/Random.h"
#include "util/RandBytes.h"
#include "util/PrimalityTest.h"
#include <math.h>
#include <stdio.h>


static const unsigned int MOST_SIGNIFICANT_WORD_FIRST = 1;
static const unsigned int NATIVE_ENDIANNESS  = 0;
static const unsigned int NO_SKIP = 0;

void random_unsignedIntOfLength(unsigned int *randomOutput, const unsigned int numberOfBits)
{
    unsigned int numberOfBytes = (numberOfBits + 7) / 8;

    unsigned char buffer[sizeof(unsigned int)] = {0};

    cryptid_randomBytes((unsigned char*) &buffer, numberOfBytes);

    unsigned int unneededBits = 8 * numberOfBytes - numberOfBits;
    buffer[numberOfBytes - 1] &= (1 << (8 - unneededBits)) - 1;

    *randomOutput = *((unsigned int*)&buffer);
}

//Using the method suggested by Johannes A. Buchmann in Introduction to Cryptography Second Edition Section 4.6
void random_unsignedIntInRange(unsigned int *randomOutput, const unsigned int range)
{
    unsigned int rangeBitLength = (int)log2(range)+1;

    do
    {
        random_unsignedIntOfLength(randomOutput, rangeBitLength);
    }while(*randomOutput > range);
}

void random_mpzOfLength(mpz_t result, const unsigned int numberOfBits)
{
    unsigned int numberOfBytes = (numberOfBits + 7) / 8;

    unsigned char buffer[numberOfBytes];

    cryptid_randomBytes((unsigned char*) &buffer, numberOfBytes);

    unsigned int unneededBits = 8 * numberOfBytes - numberOfBits;
    buffer[0] &= (1 << (8 - unneededBits)) - 1;

    mpz_import(result, sizeof (buffer), MOST_SIGNIFICANT_WORD_FIRST, sizeof (buffer[0]), NATIVE_ENDIANNESS, NO_SKIP, buffer);
}

void random_mpzInRange(mpz_t result, const mpz_t range)
{
    unsigned int rangeBitLength = mpz_sizeinbase(range, 2);
    
    do
    {
        random_mpzOfLength(result, rangeBitLength);
    }while(mpz_cmp(result, range) > 0);
}

CryptidStatus random_solinasPrime(mpz_t result, const unsigned int numberOfBits, const unsigned int attemptLimit)
{
    unsigned int random = 0, lastrandom;
    unsigned int isPrimeGenerated = 0;
    unsigned int attempts = 0;

    while ((attempts < attemptLimit) && !isPrimeGenerated)
    {
        lastrandom = random;

        random_unsignedIntInRange(&random, numberOfBits - (lastrandom + 1));
        random += lastrandom;
        for (unsigned int i = random; i > lastrandom; i--)
        {
            mpz_ui_pow_ui(result, 2, numberOfBits);
            mpz_t twoOnI;

            mpz_init(twoOnI);
            mpz_ui_pow_ui(twoOnI, 2, i);

            mpz_sub(result, result, twoOnI);
            mpz_clear(twoOnI);

            mpz_sub_ui(result, result, 1);

            if (primalityTest_isProbablePrime(result))
            {
                isPrimeGenerated = 1;
                break;
            }
        }

        attempts++;
    }

    return isPrimeGenerated ? CRYPTID_SUCCESS : CRYPTID_ATTEMPT_LIMIT_REACHED_ERROR;
}

static AffinePoint mod3PointGenerationStrategy(const EllipticCurve ellipticCurve)
{
    size_t numberOfBits = mpz_sizeinbase(ellipticCurve.fieldOrder, 2);

    mpz_t y, bAddInv, exp, ySquared, base, x;
    mpz_inits(y, bAddInv, exp, ySquared, base, x, NULL);

    random_mpzOfLength(y, numberOfBits);
    mpz_mod(y, y, ellipticCurve.fieldOrder);

    mpz_sub(bAddInv, ellipticCurve.fieldOrder, ellipticCurve.b);

    mpz_powm_ui(ySquared, y, 2, ellipticCurve.fieldOrder);

    mpz_add(base, ySquared, bAddInv);
    mpz_mod(base, base, ellipticCurve.fieldOrder);

    mpz_mul_ui(exp, ellipticCurve.fieldOrder, 2);
    mpz_sub_ui(exp, exp, 1);
    mpz_fdiv_q_ui(exp, exp, 3);

    mpz_powm(x, base, exp, ellipticCurve.fieldOrder);

    AffinePoint result;
    affine_init(&result, x, y);

    mpz_clears(y, bAddInv, exp, ySquared, base, x, NULL);

    return result;
}

CryptidStatus random_affinePoint(AffinePoint* result, const EllipticCurve ellipticCurve, const unsigned int attemptLimit)
{
    unsigned int attempts = 0;
    int isPointGenerated = 0;

    do
    {
        *result = mod3PointGenerationStrategy(ellipticCurve);

        if (affine_isOnCurve(*result, ellipticCurve))
        {
            isPointGenerated = 1;
            break;
        }

        affine_destroy(*result);

        attempts++;
    }
    while (attempts < attemptLimit);

    return isPointGenerated ? CRYPTID_SUCCESS : CRYPTID_ATTEMPT_LIMIT_REACHED_ERROR;
}
