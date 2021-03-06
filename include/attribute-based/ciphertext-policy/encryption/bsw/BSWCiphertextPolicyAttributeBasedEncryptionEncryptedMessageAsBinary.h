
#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ENCRYPTEDMESSAGE_AS_BINARY_ABE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ENCRYPTEDMESSAGE_AS_BINARY_ABE_H

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage.h"
#include "complex/ComplexAsBinary.h"

typedef struct bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary {
  ComplexAsBinary cTilde;
  struct bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary
      *cTildeSet;
  CtildeSetState last;
} bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary;

typedef struct
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary {
  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *tree;
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *cTildeSet;
  AffinePointAsBinary c;
} bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary;

void bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encrypted);

void bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *ctildeSet,
    const bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary
        *ctildeSetAsBinary);

void bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary
        *ctildeSetAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *ctildeSet);

void bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage
        *encryptedMessage,
    const bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encryptedMessageAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encryptedMessageAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage
        *encryptedMessage);

#endif
