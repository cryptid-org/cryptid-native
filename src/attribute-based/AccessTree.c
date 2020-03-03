#include "attribute-based/AccessTree.h"

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

int isLeaf(AccessTree* accessTree) {
	return (accessTree->children[0] == NULL) ? 1 : 0;
}

char** attributeArray() {
	char **attributes = malloc(MAX_ATTRIBUTES*sizeof(char)*(ATTRIBUTE_LENGTH+1));

	for(size_t i = 0; i < MAX_ATTRIBUTES; i++)
	{
	   attributes[i] = "";
	}

	return attributes;
}

int hasAttribute(char** attributes, char* val) {
	if(val != NULL)
	{
	    for(int i = 0; i < MAX_ATTRIBUTES; i++)
	    {
	        if(attributes[i] && attributes[i][0] != '\0' && strcmp(attributes[i], val) == 0)
	            return 1;
	    }
	}
    return 0;
}

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