#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_POLYNOM_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_POLYNOM_H

#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "util/Random.h"
#include "util/Status.h"

typedef struct bswCiphertextPolicyAttributeBasedEncryptionPolynomExpression {
  int degree;
  mpz_t coeff;
} bswCiphertextPolicyAttributeBasedEncryptionPolynomExpression;

typedef struct bswCiphertextPolicyAttributeBasedEncryptionPolynom {
  int degree;
  bswCiphertextPolicyAttributeBasedEncryptionPolynomExpression **children;
} bswCiphertextPolicyAttributeBasedEncryptionPolynom;

bswCiphertextPolicyAttributeBasedEncryptionPolynom *
bswCiphertextPolicyAttributeBasedEncryptionPolynom_init(
    const int degree, const mpz_t zeroValue,
    const bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey);

CryptidStatus bswCiphertextPolicyAttributeBasedEncryptionPolynomSum(
    const bswCiphertextPolicyAttributeBasedEncryptionPolynom *polynom,
    const int x, mpz_t sum);

void bswCiphertextPolicyAttributeBasedEncryptionPolynom_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionPolynom *polynom);

#endif
