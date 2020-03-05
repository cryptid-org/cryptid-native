#ifndef __CRYPTID_CRYPTID_ABEUTILS_H
#define __CRYPTID_CRYPTID_ABEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"

int Lagrange_coefficient(const int xi, const int* S, const int sLength, const int x);

char* concat(const char *s1, const char *s2);

void BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(mpz_t randElement, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

int BSWCiphertextPolicyAttributeBasedEncryptionHasAttribute(char** attributes, const int num_attributes, const char* val);

#endif
