#include "attribute-based/AccessTree.h"

// Creating an access tree as specified in Bethencourt-Sahai-Waters CP-ABE pdf
// Value is threshold
//	value = 1 meaning an OR gate
//	value = [number of childrens] meaning an AND gate
AccessTree* createTree(int value, char* attribute, size_t attributeLength) {
	AccessTree* tree = malloc(sizeof(AccessTree));
    tree->value = value;
    tree->computed = 0;
    for(int i = 0; i < MAX_CHILDREN; i++)
    {
    	tree->children[i] = NULL;
    }

    tree->attribute = attribute;
    tree->attributeLength = attributeLength;

    return tree;
}

// Returning whether a node of a tree is leaf
int isLeaf(AccessTree* accessTree) {
	return (accessTree->children[0] == NULL) ? 1 : 0;
}

// Returning 1 if attributes satisfy the accessTree, else 0
int satisfyValue(AccessTree* accessTree, char** attributes) {
	if(isLeaf(accessTree)) {
		return hasAttribute(attributes, accessTree->attribute);
	} else {
		int i;
		int counter = 0;
		for(i = 0; i < MAX_CHILDREN; i++) {
			if(accessTree->children[i] != NULL) {
				if(satisfyValue(accessTree->children[i], attributes) == 1) {
					counter++;
					if(counter >= accessTree->value) {
						return 1;
					}
				}
			}
		}
		return 0;
	}
}

// Used for deleting the tree and its children from memory
void destroyTree(AccessTree* tree)
{
	for(int i = 0; i < MAX_CHILDREN; i++)
    {
        if(tree->children[i] != NULL)
        {
        	destroyTree(tree->children[i]);
        }
    }
    if(tree->computed == 1)
    {
    	affine_destroy(tree->Cy);
    	affine_destroy(tree->CyA);
    }
    free(tree);
}