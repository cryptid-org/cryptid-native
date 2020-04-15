#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ABEUTILS_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ABEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"

int Lagrange_coefficient(const int xi, const int* s, const int sLength, const int x);

char* concat(const char *s1, const char *s2);

void BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(mpz_t randElement, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

int BSWCiphertextPolicyAttributeBasedEncryptionHasAttribute(char** attributes, const int numAttributes, const char* val);

#endif
