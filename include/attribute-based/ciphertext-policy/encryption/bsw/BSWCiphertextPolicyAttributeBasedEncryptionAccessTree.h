#ifndef __CRYPTID_CRYPTID_ACCESSTREE_H
#define __CRYPTID_CRYPTID_ACCESSTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionDefines.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionUtils.h"
#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPolynom.h"
#include "elliptic/AffinePoint.h"
#include "util/Utils.h"

typedef struct BSWCiphertextPolicyAttributeBasedEncryptionAccessTree
{
    int value;
    int computed;
    struct BSWCiphertextPolicyAttributeBasedEncryptionAccessTree** children;
    char* attribute;
    int num_children;
    size_t attributeLength;
    AffinePoint Cy;
    AffinePoint CyA;
} BSWCiphertextPolicyAttributeBasedEncryptionAccessTree;

BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_init(const int value, char* attribute, const size_t attributeLength, const int num_children);

int BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree);

int BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, char** attributes, const int num_attributes);

CryptidStatus BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, const mpz_t s, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey);

void BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* tree);

#endif