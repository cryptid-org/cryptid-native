#ifndef __CRYPTID_UTILS_H
#define __CRYPTID_UTILS_H

#include "gmp.h"

#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/HashFunction.h"
#include "util/Status.h"


void hashToRange(mpz_t result, unsigned char* s, int sLength, mpz_t p, HashFunction hashFunction);

Status hashToPoint(AffinePoint *result, EllipticCurve ellipticCurve, mpz_t p, mpz_t q, char* id, int idLength, HashFunction hashFunction);

unsigned char* canonical(int* resultLength, mpz_t p, Complex v, int order);

unsigned char* hashBytes(int b, unsigned char* p, int pLength, HashFunction hashFunction);

#endif
