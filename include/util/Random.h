#ifndef __CRYPTID_RANDOM_H
#define __CRYPTID_RANDOM_H

#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "util/Status.h"


unsigned int random_unsignedIntInRange(unsigned int range);

void random_mpzOfLength(mpz_t result, unsigned int numberOfBits);

void random_mpzInRange(mpz_t result, mpz_t range);

Status random_solinasPrime(mpz_t result, unsigned int numberOfBits, unsigned int attemptLimit);

Status random_affinePoint(AffinePoint* result, EllipticCurve ellipticCurve, unsigned int attemptLimit);

#endif
