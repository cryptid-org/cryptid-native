#include <stdlib.h>
#include <string.h>

#include "elliptic/EllipticCurveAsString.h"

void ellipticCurveAsString_init(EllipticCurveAsString *ellipticCurveAsStringOutput, const char *const a, const int aBase, const char *const b, const int bBase, const char *const fieldOrder, const int fieldOrderBase)
{
    int aLength = strlen(a);
    ellipticCurveAsStringOutput->a = (char*)malloc(aLength * sizeof(char));
    memcpy(ellipticCurveAsStringOutput->a, a, aLength);

    ellipticCurveAsStringOutput->aBase = aBase;

    int bLength = strlen(b);
    ellipticCurveAsStringOutput->b = (char*)malloc(bLength * sizeof(char));
    memcpy(ellipticCurveAsStringOutput->b, b, bLength);

    ellipticCurveAsStringOutput->bBase = bBase;

    int fieldOrderLength = strlen(fieldOrder);
    ellipticCurveAsStringOutput->fieldOrder = (char*)malloc(fieldOrderLength * sizeof(char));
    memcpy(ellipticCurveAsStringOutput->fieldOrder, fieldOrder, fieldOrderLength);

    ellipticCurveAsStringOutput->fieldOrderBase = fieldOrderBase;
}

void ellipticCurveAsString_destroy(EllipticCurveAsString ellipticCurveAsString)
{
    free(ellipticCurveAsString.a);
    free(ellipticCurveAsString.b);
    free(ellipticCurveAsString.fieldOrder);
}

void ellipticCurveAsString_toEllipticCurve(EllipticCurve *ellipticCurveOutput, const EllipticCurveAsString ellipticCurveAsString)
{
    mpz_inits(ellipticCurveOutput->a,ellipticCurveOutput->b, ellipticCurveOutput->fieldOrder, NULL);

    mpz_set_str(ellipticCurveOutput->a, ellipticCurveAsString.a, ellipticCurveAsString.aBase);
    mpz_set_str(ellipticCurveOutput->b, ellipticCurveAsString.b, ellipticCurveAsString.bBase);
    mpz_set_str(ellipticCurveOutput->fieldOrder, ellipticCurveAsString.fieldOrder, ellipticCurveAsString.fieldOrderBase);
}

void ellipticCurve_toEllipticCurveAsString(EllipticCurveAsString *ellipticCurveAsStringOutput, const EllipticCurve ellipticCurve, const int aBase, const int bBase, const int fieldOrderBase)
{
    ellipticCurveAsStringOutput->a = mpz_get_str(NULL, aBase, ellipticCurve.a);

    ellipticCurveAsStringOutput->aBase = aBase;

    ellipticCurveAsStringOutput->b = mpz_get_str(NULL, bBase, ellipticCurve.b);

    ellipticCurveAsStringOutput->bBase = bBase;

    ellipticCurveAsStringOutput->fieldOrder = mpz_get_str(NULL, fieldOrderBase, ellipticCurve.fieldOrder);

    ellipticCurveAsStringOutput->fieldOrderBase = fieldOrderBase;
}