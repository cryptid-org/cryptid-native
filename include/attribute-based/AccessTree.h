#ifndef __CRYPTID_CRYPTID_ACCESSTREE_H
#define __CRYPTID_CRYPTID_ACCESSTREE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attribute-based/Defines.h"
#include "elliptic/AffinePoint.h"

typedef struct AccessTree {
    int value;
    struct AccessTree* parent;
    struct AccessTree* children[MAX_CHILDREN];
    char* attribute;
    size_t attributeLength;
    AffinePoint Cy;
    AffinePoint CyA;
    //Polynom* polynom;
} AccessTree;

int isRoot(AccessTree* accessTree);

AccessTree* createTree(int value, AccessTree* children, char* attribute, const size_t attributeLength);

int isLeaf(AccessTree* accessTree);

AccessTree* childrenArray();

char** attributeArray();

int hasAttribute(char** attributes, char* val);

int satisfyValue(AccessTree* accessTree, char** attributes);

#endif