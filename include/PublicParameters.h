#ifndef PUBLICPARAMETERS_H
#define PUBLICPARAMETERS_H

#include "gmp.h"

#include "AffinePoint.h"
#include "EllipticCurve.h"
#include "HashFunction.h"

typedef struct PublicParameters
{
    EllipticCurve ellipticCurve;
    mpz_t q;
    AffinePoint pointP;
    AffinePoint pointPpublic;
    HashFunction hashFunction;
} PublicParameters;

#endif
