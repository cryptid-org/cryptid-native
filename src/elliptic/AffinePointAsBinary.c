#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "elliptic/AffinePointAsBinary.h"

void affineAsBinary_init(AffinePointAsBinary *affinePointAsBinaryOutput, const void *const x, const size_t xLength, const void *const y, const size_t yLength)
{
    affinePointAsBinaryOutput->x = malloc(xLength + 1);
    memcpy(affinePointAsBinaryOutput->x, x, xLength + 1);

    affinePointAsBinaryOutput->xLength = xLength;

    affinePointAsBinaryOutput->y = malloc(yLength + 1);
    memcpy(affinePointAsBinaryOutput->y, y, yLength + 1);

    affinePointAsBinaryOutput->yLength = yLength;
}

void affineAsBinary_destroy(AffinePointAsBinary affinePointAsBinary)
{
    free(affinePointAsBinary.x);
    free(affinePointAsBinary.y);
}

void affineAsBinary_toAffine(AffinePoint *affinePointOutput, const AffinePointAsBinary affinePointAsBinary)
{
    mpz_inits(affinePointOutput->x, affinePointOutput->y, NULL);
    mpz_import(affinePointOutput->x, affinePointAsBinary.xLength, 1, 1, 0, 0, affinePointAsBinary.x);
    mpz_import(affinePointOutput->y, affinePointAsBinary.yLength, 1, 1, 0, 0, affinePointAsBinary.y);
}

void affineAsBinary_fromAffine(AffinePointAsBinary *affinePointAsBinaryOutput, const AffinePoint affinePoint)
{
    affinePointAsBinaryOutput->x = mpz_export(NULL, &affinePointAsBinaryOutput->xLength, 1, 1, 0, 0, affinePoint.x);

    affinePointAsBinaryOutput->y = mpz_export(NULL, &affinePointAsBinaryOutput->yLength, 1, 1, 0, 0, affinePoint.y);
}