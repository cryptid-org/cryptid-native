#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_AS_BINARY_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_AS_BINARY_H

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/AffinePointAsBinary.h"
#include "util/Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary {
  int value;
  int computed;
  struct bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *
      *children;
  void *attribute;
  int numChildren;
  size_t attributeLength;
  AffinePointAsBinary cY;
  AffinePointAsBinary cYa;
} bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary;

bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *
bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
    const int value, char *attribute, const size_t attributeLength,
    const int numChildren);

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
        *accessTreeAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree,
    const bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
        *accessTreeAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
        *accessTreeAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree);

#endif