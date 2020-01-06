#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 128
#define MAX_ATTRIBUTES 128
#define ATTRIBUTE_LENGTH 32

typedef struct AccessTree {
    int value;
    struct AccessTree* parent;
    struct AccessTree* children[MAX_CHILDREN];
    char* attribute;
} AccessTree;

AccessTree* createTree(int value, AccessTree* children) {
	AccessTree* tree = malloc(sizeof(AccessTree));
    tree->value = value;
    memcpy(tree->children, children, sizeof(tree->children));
    tree->parent = NULL;
}

int isLeaf(AccessTree* accessTree) {
	return (accessTree->children[0] == NULL) ? 1 : 0;
}

AccessTree* childrenArray() {
	AccessTree children[MAX_CHILDREN] = {};
	AccessTree *c_p = children;
	memset(children, '\0', sizeof(children));
	return c_p;
}

char** attributeArray() {
	char **attributes = malloc(MAX_ATTRIBUTES);

	for(size_t i = 0; i < MAX_ATTRIBUTES; i++){
	   attributes[i] = malloc(ATTRIBUTE_LENGTH);
	}

	return attributes;
}

int hasAttribute(char** attributes, char* val) {
    for(int i = 0; i < MAX_ATTRIBUTES; i++)
    {
        if(attributes[i] != '\0' && strcmp(attributes[i], val) == 0)
            return 1;
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