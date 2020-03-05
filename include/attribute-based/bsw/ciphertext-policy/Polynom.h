#ifndef __CRYPTID_CRYPTID_POLYNOM_H
#define __CRYPTID_CRYPTID_POLYNOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/bsw/ciphertext-policy/ABE_Utils.h"
#include "attribute-based/bsw/ciphertext-policy/Defines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/bsw/ciphertext-policy/MasterKey_ABE.h"

typedef struct Polynom
{
	int degree;
	struct PolynomExpression** children;
} Polynom;

typedef struct PolynomExpression
{
	int degree;
	mpz_t coeff;
} PolynomExpression;

Polynom* createPolynom(const int degree, const mpz_t zeroValue, const PublicKey_ABE* publickey);

CryptidStatus polynomSum(const Polynom* polynom, const int x, mpz_t sum);

void destroyPolynom(Polynom* polynom);

#endif
