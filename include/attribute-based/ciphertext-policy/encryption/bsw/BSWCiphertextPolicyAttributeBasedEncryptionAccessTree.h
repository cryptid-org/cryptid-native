#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "elliptic/AffinePoint.h"
#include "util/Utils.h"

typedef struct bswCiphertextPolicyAttributeBasedEncryptionAccessTree
{
    int value;
    int computed;
    struct bswCiphertextPolicyAttributeBasedEncryptionAccessTree** children;
    int numChildren;
    char* attribute;
    size_t attributeLength;
    AffinePoint cY;
    AffinePoint cYa;
} bswCiphertextPolicyAttributeBasedEncryptionAccessTree;

bswCiphertextPolicyAttributeBasedEncryptionAccessTree* bswCiphertextPolicyAttributeBasedEncryptionAccessTree_init(const int value, char* attribute, const size_t attributeLength, const int numChildren);

int bswCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(const bswCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree);

int bswCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(const bswCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, char** attributes, const int numAttributes);

CryptidStatus bswCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(bswCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, const mpz_t s, const bswCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

void bswCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(bswCiphertextPolicyAttributeBasedEncryptionAccessTree* tree);

#endif