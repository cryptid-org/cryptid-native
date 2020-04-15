#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_AS_BINARY_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_AS_BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/AffinePointAsBinary.h"
#include "util/Utils.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
{
    int value;
    int computed;
    struct BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary** children;
    void *attribute;
    int numChildren;
    size_t attributeLength;
    AffinePointAsBinary cY;
    AffinePointAsBinary cYa;
} BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary;

BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(const int value, char* attribute, const size_t attributeLength, const int numChildren);

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *accessTreeAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree, const BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *accessTreeAsBinary);

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *accessTreeAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree);

#endif