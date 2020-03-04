#ifndef __CRYPTID_CRYPTID_POLYNOM_H
#define __CRYPTID_CRYPTID_POLYNOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/Defines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/MasterKey_ABE.h"

typedef struct Polynom {
	int degree;
    struct PolynomExpression** children;
} Polynom;

typedef struct PolynomExpression {
	int degree;
	mpz_t coeff;
} PolynomExpression;

CryptidStatus ABE_randomNumber(mpz_t randElement, PublicKey_ABE* publickey);

Polynom* createPolynom(int degree, mpz_t zeroValue, PublicKey_ABE* publickey);

CryptidStatus polynomSum(Polynom* polynom, int x, mpz_t sum);

void destroyPolynom(Polynom* polynom);

#endif
