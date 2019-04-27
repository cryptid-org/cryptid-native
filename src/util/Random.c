#include "util/Random.h"
#include "util/RandBytes.h"
#include "util/Validation.h"


static const unsigned int MOST_SIGNIFICANT_WORD_FIRST = 1;
static const unsigned int NATIVE_ENDIANNESS  = 0;
static const unsigned int NO_SKIP = 0;

unsigned int random_unsignedIntInRange(const unsigned int range)
{
    unsigned int x, r;

    do
    {
        randomBytes((unsigned char*) &x, sizeof (x));

        r = x % range;
    }
    while (x - r > (-range));

    return r;
}

void random_mpzOfLength(mpz_t result, const unsigned int numberOfBits)
{
    unsigned int numberOfBytes = (numberOfBits + 7) / 8;

    unsigned char buffer[numberOfBytes];

    randomBytes((unsigned char*) &buffer, numberOfBytes);

    unsigned int unneededBits = 8 * numberOfBytes - numberOfBits;
    buffer[0] &= (1 << (8 - unneededBits)) - 1;

    mpz_import(result, sizeof (buffer), MOST_SIGNIFICANT_WORD_FIRST, sizeof (buffer[0]), NATIVE_ENDIANNESS, NO_SKIP, buffer);
}

void random_mpzInRange(mpz_t result, const mpz_t range)
{
    mpz_t x, r, xMinusR, negativeRange;
    mpz_inits(x, r, xMinusR, negativeRange, NULL);

    mpz_ui_pow_ui(negativeRange, 2, mpz_sizeinbase(range, 2));
    mpz_sub(negativeRange, negativeRange, range);

    do
    {
        random_mpzOfLength(x, mpz_sizeinbase(range, 2));

        mpz_mod(r, x, range);

        mpz_sub(xMinusR, x, r);
    }
    while (mpz_cmp(xMinusR, negativeRange) > 0);

    mpz_set(result, r);

    mpz_clears(x, r, xMinusR, negativeRange, NULL);
}

Status random_solinasPrime(mpz_t result, const unsigned int numberOfBits, const unsigned int attemptLimit)
{
    unsigned int random = 1, lastrandom;
    unsigned int isPrimeGenerated = 0;
    unsigned int attempts = 0;

    while ((attempts < attemptLimit) && !isPrimeGenerated)
    {
        lastrandom = random;

        random = random_unsignedIntInRange(numberOfBits - lastrandom) + lastrandom;

        for (unsigned int i = random; i >= lastrandom; i--)
        {
            mpz_ui_pow_ui(result, 2, numberOfBits);
            mpz_t twoOnI;

            mpz_init(twoOnI);
            mpz_ui_pow_ui(twoOnI, 2, i);

            mpz_sub(result, result, twoOnI);
            mpz_clear(twoOnI);

            mpz_sub_ui(result, result, 1);

            if (validation_isProbablePrime(result))
            {
                isPrimeGenerated = 1;
                break;
            }
        }

        attempts++;
    }

    return isPrimeGenerated ? SUCCESS : ATTEMPT_LIMIT_REACHED_ERROR;
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

    AffinePoint result = affine_init(x, y);

    mpz_clears(y, bAddInv, exp, ySquared, base, x, NULL);

    return result;
}

Status random_affinePoint(AffinePoint* result, const EllipticCurve ellipticCurve, const unsigned int attemptLimit)
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

    return isPointGenerated ? SUCCESS : ATTEMPT_LIMIT_REACHED_ERROR;
}
