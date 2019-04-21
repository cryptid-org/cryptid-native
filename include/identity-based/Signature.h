#ifndef __CRYPTID_SIGNATURE_H
#define __CRYPTID_SIGNATURE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"


typedef struct Signature
{
    AffinePoint cipherU;
    mpz_t cipherV;
} Signature;

Signature signature_init(AffinePoint cipherU, mpz_t cipherV);

void signature_destroy(Signature signature);

#endif
