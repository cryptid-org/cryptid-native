#include "gmp.h"

#include "elliptic/EllipticCurve.h"


void ellipticCurve_init(EllipticCurve *ellipticCurveOutput, const mpz_t a, const mpz_t b, const mpz_t fieldOrder)
{
    mpz_inits(ellipticCurveOutput->a,ellipticCurveOutput->b, ellipticCurveOutput->fieldOrder, NULL);

    mpz_set(ellipticCurveOutput->a, a);
    mpz_set(ellipticCurveOutput->b, b);
    mpz_set(ellipticCurveOutput->fieldOrder, fieldOrder);
}

void ellipticCurve_initLong(EllipticCurve *ellipticCurveOutput, const long a, const long b, const long fieldOrder)
{
    mpz_inits(ellipticCurveOutput->a, ellipticCurveOutput->b, ellipticCurveOutput->fieldOrder, NULL);

    mpz_set_si(ellipticCurveOutput->a, a);
    mpz_set_si(ellipticCurveOutput->b, b);
    mpz_set_si(ellipticCurveOutput->fieldOrder, fieldOrder);
}

void ellipticCurve_destroy(EllipticCurve ellipticCurve)
{
    mpz_clears(ellipticCurve.a, ellipticCurve.b, ellipticCurve.fieldOrder, NULL);
}
