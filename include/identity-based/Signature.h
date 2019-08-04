#ifndef __CRYPTID_SIGNATURE_H
#define __CRYPTID_SIGNATURE_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"


typedef struct Signature
{
    AffinePoint u;
    mpz_t v;
} Signature;

Signature signature_init(AffinePoint u, mpz_t v);

void signature_destroy(Signature signature);

#endif
