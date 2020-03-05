#ifndef __CRYPTID_CRYPTID_ACCESSTREE_H
#define __CRYPTID_CRYPTID_ACCESSTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/bsw/ciphertext-policy/Defines.h"
#include "attribute-based/bsw/ciphertext-policy/ABE_Utils.h"
#include "elliptic/AffinePoint.h"

typedef struct AccessTree
{
    int value;
    int computed;
    struct AccessTree** children;
    char* attribute;
    int num_children;
    size_t attributeLength;
    AffinePoint Cy;
    AffinePoint CyA;
} AccessTree;

AccessTree* createTree(const int value, char* attribute, const size_t attributeLength, const int num_children);

int isLeaf(const AccessTree* accessTree);

int satisfyValue(const AccessTree* accessTree, char** attributes, const int num_attributes);

void destroyTree(AccessTree* tree);

#endif