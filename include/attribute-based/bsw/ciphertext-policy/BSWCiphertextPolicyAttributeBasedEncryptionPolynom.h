#ifndef __CRYPTID_CRYPTID_POLYNOM_H
#define __CRYPTID_CRYPTID_POLYNOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "util/Random.h"
#include "util/Status.h"
#include "attribute-based/bsw/ciphertext-policy/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionPolynom
{
	int degree;
	struct BSWCiphertextPolicyAttributeBasedEncryptionPolynomExpression** children;
} BSWCiphertextPolicyAttributeBasedEncryptionPolynom;

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionPolynomExpression
{
	int degree;
	mpz_t coeff;
} BSWCiphertextPolicyAttributeBasedEncryptionPolynomExpression;

BSWCiphertextPolicyAttributeBasedEncryptionPolynom* BSWCiphertextPolicyAttributeBasedEncryptionPolynom_init(const int degree, const mpz_t zeroValue, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

CryptidStatus BSWCiphertextPolicyAttributeBasedEncryptionPolynomSum(const BSWCiphertextPolicyAttributeBasedEncryptionPolynom* polynom, const int x, mpz_t sum);

void BSWCiphertextPolicyAttributeBasedEncryptionPolynom_destroy(BSWCiphertextPolicyAttributeBasedEncryptionPolynom* polynom);

#endif
