#ifndef __CRYPTID_CRYPTID_ABEUTILS_H
#define __CRYPTID_CRYPTID_ABEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/bsw/ciphertext-policy/Defines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/bsw/ciphertext-policy/PublicKey_ABE.h"
#include "attribute-based/bsw/ciphertext-policy/MasterKey_ABE.h"

int Lagrange_coefficient(const int xi, const int* S, const int sLength, const int x);

char* concat(const char *s1, const char *s2);

void ABE_randomNumber(mpz_t randElement, const PublicKey_ABE* publickey);

int hasAttribute(char** attributes, const int num_attributes, const char* val);

#endif
