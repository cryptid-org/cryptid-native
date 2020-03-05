#include "attribute-based/bsw/ciphertext-policy/AccessTree.h"

// Creating an access tree as specified in Bethencourt-Sahai-Waters CP-ABE pdf
// Value is threshold
//	value = 1 meaning an OR gate
//	value = [number of childrens] meaning an AND gate
AccessTree* createTree(const int value, char* attribute, const size_t attributeLength, const int num_children)
{
	AccessTree* tree = malloc(sizeof(AccessTree));
    tree->value = value;
    tree->computed = 0;
    if(num_children > 0)
    {
        tree->children = malloc(sizeof(AccessTree*) * num_children);
    }
    tree->num_children = num_children;

    tree->attribute = attribute;
    tree->attributeLength = attributeLength;

    return tree;
}

// Returning whether a node of a tree is leaf
int isLeaf(const AccessTree* accessTree)
{
	return (accessTree->num_children == 0) ? 1 : 0;
}

// Returning 1 if attributes satisfy the accessTree, else 0
int satisfyValue(const AccessTree* accessTree, char** attributes, const int num_attributes)
{
	if(isLeaf(accessTree))
    {
		return hasAttribute(attributes, num_attributes, accessTree->attribute);
	}
    
	int i;
	int counter = 0;
	for(i = 0; i < accessTree->num_children; i++)
    {
		if(satisfyValue(accessTree->children[i], attributes, num_attributes))
        {
			counter++;
			if(counter >= accessTree->value)
            {
				return 1;
			}
		}
	}
	return 0;
}

// Used for deleting the tree and its children from memory
void destroyTree(AccessTree* tree)
{
	for(int i = 0; i < tree->num_children; i++)
    {
        destroyTree(tree->children[i]);
    }
    if(tree->computed)
    {
    	affine_destroy(tree->Cy);
    	affine_destroy(tree->CyA);
    }
    if(tree->num_children > 0)
    {
        free(tree->children);
    }
    free(tree);
}