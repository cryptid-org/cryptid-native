#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_POLYNOM_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_POLYNOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionPolynomExpression
{
	int degree;
	mpz_t coeff;
} BSWCiphertextPolicyAttributeBasedEncryptionPolynomExpression;

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionPolynom
{
	int degree;
	BSWCiphertextPolicyAttributeBasedEncryptionPolynomExpression** children;
} BSWCiphertextPolicyAttributeBasedEncryptionPolynom;

BSWCiphertextPolicyAttributeBasedEncryptionPolynom* BSWCiphertextPolicyAttributeBasedEncryptionPolynom_init(const int degree, const mpz_t zeroValue, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

CryptidStatus BSWCiphertextPolicyAttributeBasedEncryptionPolynomSum(const BSWCiphertextPolicyAttributeBasedEncryptionPolynom* polynom, const int x, mpz_t sum);

void BSWCiphertextPolicyAttributeBasedEncryptionPolynom_destroy(BSWCiphertextPolicyAttributeBasedEncryptionPolynom* polynom);

#endif
