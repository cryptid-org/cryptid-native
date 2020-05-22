#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_MASTERKEY_ABE_AS_BINARY_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_MASTERKEY_ABE_AS_BINARY_H

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKey.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary.h"
#include "elliptic/AffinePointAsBinary.h"
#include <stdio.h>

typedef struct bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary {
  void *beta;
  size_t betaLength;
  AffinePointAsBinary g_alpha;
  bswCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickey;
} bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary;

void bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterkey);

void bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionMasterKey(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKey *masterKey,
    const bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary
        *masterKeyAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionMasterKey(
    bswCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary
        *masterKeyAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionMasterKey *masterKey);

#endif
