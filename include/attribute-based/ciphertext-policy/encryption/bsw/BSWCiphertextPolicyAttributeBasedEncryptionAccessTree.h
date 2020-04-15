#ifndef __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_H
#define __CRYPTID_BSW_CIPHERTEXT_POLICY_ATTRIBUTE_BASED_ENCRYPTION_ACCESS_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "elliptic/AffinePoint.h"
#include "util/Utils.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionAccessTree
{
    int value;
    int computed;
    struct BSWCiphertextPolicyAttributeBasedEncryptionAccessTree** children;
    int numChildren;
    char* attribute;
    size_t attributeLength;
    AffinePoint cY;
    AffinePoint cYa;
} BSWCiphertextPolicyAttributeBasedEncryptionAccessTree;

BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_init(const int value, char* attribute, const size_t attributeLength, const int numChildren);

int BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree);

int BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, char** attributes, const int numAttributes);

CryptidStatus BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, const mpz_t s, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

void BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* tree);

#endif