
#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ENCRYPTEDMESSAGE_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ENCRYPTEDMESSAGE_ABE_H

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "gmp.h"

typedef struct bswCiphertextPolicyAttributeBasedEncryptionCtildeSet {
  Complex cTilde;
  struct bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *cTildeSet;
  CtildeSetState last;
} bswCiphertextPolicyAttributeBasedEncryptionCtildeSet;

typedef struct bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage {
  bswCiphertextPolicyAttributeBasedEncryptionAccessTree *tree;
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *cTildeSet;
  AffinePoint c;
} bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage;

void bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage *encrypted);

#endif
