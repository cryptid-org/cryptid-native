#include "gmp.h"

#include "elliptic/EllipticCurve.h"


EllipticCurve ellipticCurve_init(const mpz_t a, const mpz_t b, const mpz_t fieldOrder)
{
    EllipticCurve ellipticCurve;

    mpz_inits(ellipticCurve.a, ellipticCurve.b, ellipticCurve.fieldOrder, NULL);

    mpz_set(ellipticCurve.a, a);
    mpz_set(ellipticCurve.b, b);
    mpz_set(ellipticCurve.fieldOrder, fieldOrder);

    return ellipticCurve;
}

EllipticCurve ellipticCurve_initLong(const long a, const long b, const long fieldOrder)
{
    EllipticCurve ellipticCurve;

    mpz_inits(ellipticCurve.a, ellipticCurve.b, ellipticCurve.fieldOrder, NULL);

    mpz_set_si(ellipticCurve.a, a);
    mpz_set_si(ellipticCurve.b, b);
    mpz_set_si(ellipticCurve.fieldOrder, fieldOrder);

    return ellipticCurve;
}

void ellipticCurve_destroy(EllipticCurve ellipticCurve)
{
    mpz_clears(ellipticCurve.a, ellipticCurve.b, ellipticCurve.fieldOrder, NULL);
}
