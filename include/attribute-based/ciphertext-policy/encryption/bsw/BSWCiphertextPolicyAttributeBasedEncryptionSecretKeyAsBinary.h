#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_SECRETKEY_AS_BINARY_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_SECRETKEY_AS_BINARY_ABE_H
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionSecretKey.h"
#include "elliptic/AffinePointAsBinary.h"
#include <string.h>

typedef struct bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary {
  AffinePointAsBinary d;
  AffinePointAsBinary *dJ;
  AffinePointAsBinary *dJa;
  void **attributes;
  int *attributeLengths;
  int numAttributes;
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey;
} bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary;

void bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *secretkey);

void bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionSecretKey(
    bswCiphertextPolicyAttributeBasedEncryptionSecretKey *secretKey,
    const bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
        *secretKeyAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionSecretKey(
    bswCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary
        *secretKeyAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionSecretKey *secretKey);

#endif
