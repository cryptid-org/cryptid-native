#ifndef __CRYPTID_PUBLICPARAMETERS_H
#define __CRYPTID_PUBLICPARAMETERS_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/HashFunction.h"


typedef struct PublicParameters
{
    EllipticCurve ellipticCurve;
    mpz_t q;
    AffinePoint pointP;
    AffinePoint pointPpublic;
    HashFunction hashFunction;
} PublicParameters;

#endif
