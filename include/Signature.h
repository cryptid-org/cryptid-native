#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "gmp.h"
#include "AffinePoint.h"

typedef struct Signature
{
    AffinePoint cipherU;
    mpz_t cipherV;
} Signature;

Signature signature_init(AffinePoint cipherU, mpz_t cipherV);

void signature_destroy(Signature signature);

#endif
