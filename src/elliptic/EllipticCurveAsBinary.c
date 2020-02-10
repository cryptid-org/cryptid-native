#include <stdlib.h>
#include <string.h>

#include "elliptic/EllipticCurveAsBinary.h"

void ellipticCurveAsBinary_init(EllipticCurveAsBinary *ellipticCurveAsBinaryOutput, const char *const a, const size_t aLength, const char *const b, const size_t bLength, const char *const fieldOrder, const size_t fieldOrderLength)
{
    ellipticCurveAsBinaryOutput->a = (char*)malloc(aLength * sizeof(char) + 1);
    memcpy(ellipticCurveAsBinaryOutput->a, a, aLength + 1);

    ellipticCurveAsBinaryOutput->aLength = aLength;

    ellipticCurveAsBinaryOutput->b = (char*)malloc(bLength * sizeof(char) + 1);
    memcpy(ellipticCurveAsBinaryOutput->b, b, bLength + 1);

    ellipticCurveAsBinaryOutput->bLength = bLength;

    ellipticCurveAsBinaryOutput->fieldOrder = (char*)malloc(fieldOrderLength * sizeof(char) + 1);
    memcpy(ellipticCurveAsBinaryOutput->fieldOrder, fieldOrder, fieldOrderLength + 1);

    ellipticCurveAsBinaryOutput->fieldOrderLength = fieldOrderLength;
}

void ellipticCurveAsBinary_destroy(EllipticCurveAsBinary ellipticCurveAsBinary)
{
    free(ellipticCurveAsBinary.a);
    free(ellipticCurveAsBinary.b);
    free(ellipticCurveAsBinary.fieldOrder);
}

void ellipticCurveAsBinary_toEllipticCurve(EllipticCurve *ellipticCurveOutput, const EllipticCurveAsBinary ellipticCurveAsBinary)
{
    mpz_inits(ellipticCurveOutput->a, ellipticCurveOutput->b, ellipticCurveOutput->fieldOrder, NULL);

    mpz_import(ellipticCurveOutput->a, ellipticCurveAsBinary.aLength, 1, 1, 0, 0, ellipticCurveAsBinary.a);
    mpz_import(ellipticCurveOutput->b, ellipticCurveAsBinary.bLength, 1, 1, 0, 0, ellipticCurveAsBinary.b);
    mpz_import(ellipticCurveOutput->fieldOrder, ellipticCurveAsBinary.fieldOrderLength, 1, 1, 0, 0, ellipticCurveAsBinary.fieldOrder);
}

void ellipticCurveAsBinary_fromEllipticCurve(EllipticCurveAsBinary *ellipticCurveAsBinaryOutput, const EllipticCurve ellipticCurve)
{
    ellipticCurveAsBinaryOutput->a = mpz_export(NULL, &ellipticCurveAsBinaryOutput->aLength, 1, 1, 0, 0, ellipticCurve.a);

    ellipticCurveAsBinaryOutput->b = mpz_export(NULL, &ellipticCurveAsBinaryOutput->bLength, 1, 1, 0, 0, ellipticCurve.b);

    ellipticCurveAsBinaryOutput->fieldOrder = mpz_export(NULL, &ellipticCurveAsBinaryOutput->fieldOrderLength, 1, 1, 0, 0, ellipticCurve.fieldOrder);
}