#ifndef __CRYPTID_CRYPTID_ABEUTILS_H
#define __CRYPTID_CRYPTID_ABEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/Defines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/PublicKey_ABE.h"
#include "attribute-based/MasterKey_ABE.h"

int Lagrange_coefficient(int xi, int* S, int sLength, int x);

char* concat(const char *s1, const char *s2);

CryptidStatus ABE_randomNumber(mpz_t randElement, PublicKey_ABE* publickey);

char** attributeArray();

int hasAttribute(char** attributes, char* val);

#endif
