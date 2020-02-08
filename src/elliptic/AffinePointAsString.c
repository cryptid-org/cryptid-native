#include <stdlib.h>
#include <string.h>

#include "gmp.h"

#include "elliptic/AffinePointAsString.h"

void affineAsString_init(AffinePointAsString *affinePointAsStringOutput, const char *const x, const int xBase, const char *const y, const int yBase)
{
    int xLength = strlen(x);
    affinePointAsStringOutput->x = (char*)malloc(xLength * sizeof(char));
    memcpy(affinePointAsStringOutput->x, x, xLength);

    affinePointAsStringOutput->xBase = xBase;

    int yLength = strlen(y);
    affinePointAsStringOutput->y = (char*)malloc(yLength * sizeof(char));
    memcpy(affinePointAsStringOutput->y, y, yLength);

    affinePointAsStringOutput->yBase = yBase;
}

void affineAsString_destroy(AffinePointAsString affinePointAsString)
{
    free(affinePointAsString.x);
    free(affinePointAsString.y);
}

void affineAsString_toAffine(AffinePoint *affinePointOutput, const AffinePointAsString affinePointAsString)
{
    mpz_inits(affinePointOutput->x, affinePointOutput->y, NULL);
    mpz_set_str(affinePointOutput->x, affinePointAsString.x, affinePointAsString.xBase);
    mpz_set_str(affinePointOutput->y, affinePointAsString.y, affinePointAsString.yBase);
}

void affine_toAffineAsString(AffinePointAsString *affinePointAsStringOutput, const AffinePoint affinePoint, const int xBase, const int yBase)
{
    affinePointAsStringOutput->x = mpz_get_str(NULL, xBase, affinePoint.x);

    affinePointAsStringOutput->xBase = xBase;

    affinePointAsStringOutput->y = mpz_get_str(NULL, yBase, affinePoint.y);

    affinePointAsStringOutput->yBase = yBase;
}