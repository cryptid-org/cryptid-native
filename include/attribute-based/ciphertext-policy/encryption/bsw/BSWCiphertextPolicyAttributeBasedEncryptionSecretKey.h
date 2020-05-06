#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_SECRETKEY_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_SECRETKEY_ABE_H
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKey.h"
#include "elliptic/AffinePoint.h"
#include "gmp.h"

typedef struct bswCiphertextPolicyAttributeBasedEncryptionSecretKey {
  AffinePoint d;
  AffinePoint *dJ;
  AffinePoint *dJa;
  char **attributes;
  int numAttributes;
  bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey;
} bswCiphertextPolicyAttributeBasedEncryptionSecretKey;

void bswCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionSecretKey *secretkey);

#endif
