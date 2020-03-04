#ifndef __CRYPTID_CRYPTID_ACCESSTREE_H
#define __CRYPTID_CRYPTID_ACCESSTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/Defines.h"
#include "attribute-based/ABE_Utils.h"
#include "elliptic/AffinePoint.h"

typedef struct AccessTree {
    int value;
    int computed;
    struct AccessTree* children[MAX_CHILDREN];
    char* attribute;
    size_t attributeLength;
    AffinePoint Cy;
    AffinePoint CyA;
} AccessTree;

AccessTree* createTree(int value, char* attribute, const size_t attributeLength);

int isLeaf(AccessTree* accessTree);

int satisfyValue(AccessTree* accessTree, char** attributes);

void destroyTree(AccessTree* tree);

#endif