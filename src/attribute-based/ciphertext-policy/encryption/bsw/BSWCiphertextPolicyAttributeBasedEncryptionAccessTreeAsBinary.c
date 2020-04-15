#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary.h"

BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(const int value, char* attribute, const size_t attributeLength, const int numChildren)
{
	BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* tree = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary));

    tree->value = value;

	tree->computed = 0;

    if(numChildren > 0)
    {
        tree->children = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree*) * numChildren);
    }

    tree->numChildren = numChildren;

    tree->attributeLength = attributeLength;
    if(attributeLength > 0)
    {
	    tree->attribute = malloc(attributeLength + 1);
	    memcpy(tree->attribute, attribute, attributeLength + 1);
	}

    return tree;
}

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *accessTreeAsBinary)
{
	if(accessTreeAsBinary->computed)
	{
		affineAsBinary_destroy(accessTreeAsBinary->cY);
		affineAsBinary_destroy(accessTreeAsBinary->cYa);
	}

	int numChildren = accessTreeAsBinary->numChildren;
	for(int i = 0; i < numChildren; i++)
	{
		bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(accessTreeAsBinary->children[i]);
	}
	if(numChildren > 0)
	{
		free(accessTreeAsBinary->children);
	}

	if(accessTreeAsBinary->attributeLength > 0)
	{
		free(accessTreeAsBinary->attribute);
	}

	free(accessTreeAsBinary);
}

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree, const BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *accessTreeAsBinary)
{
	accessTree->value = accessTreeAsBinary->value;
	
	accessTree->computed = accessTreeAsBinary->computed;

	accessTree->numChildren = accessTreeAsBinary->numChildren;

	accessTree->attributeLength = accessTreeAsBinary->attributeLength;

	if(accessTree->computed)
	{
		affineAsBinary_toAffine(&accessTree->cY, accessTreeAsBinary->cY);
		affineAsBinary_toAffine(&accessTree->cYa, accessTreeAsBinary->cYa);
	}

	if(accessTree->attributeLength > 0)
	{
	    accessTree->attribute = malloc(accessTree->attributeLength + 1);
	    memcpy(accessTree->attribute, accessTreeAsBinary->attribute, accessTree->attributeLength + 1);
	}
	else
	{
		accessTree->attribute = NULL;
	}

    if(accessTree->numChildren > 0)
    {
        accessTree->children = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree*) * accessTree->numChildren);
    }

	for(int i = 0; i < accessTree->numChildren; i++)
	{
		accessTree->children[i] = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree));
		bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(accessTree->children[i], accessTreeAsBinary->children[i]);
	}
}

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *accessTreeAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree)
{
	accessTreeAsBinary->value = accessTree->value;
	
	accessTreeAsBinary->computed = accessTree->computed;

	accessTreeAsBinary->numChildren = accessTree->numChildren;

	accessTreeAsBinary->attributeLength = accessTree->attributeLength;

	if(accessTree->computed)
	{
		affineAsBinary_fromAffine(&accessTreeAsBinary->cY, accessTree->cY);
		affineAsBinary_fromAffine(&accessTreeAsBinary->cYa, accessTree->cYa);
	}

	if(accessTree->attributeLength > 0)
	{
	    accessTreeAsBinary->attribute = malloc(accessTree->attributeLength + 1);
	    memcpy(accessTreeAsBinary->attribute, accessTree->attribute, accessTree->attributeLength + 1);
	}

    if(accessTree->numChildren > 0)
    {
        accessTreeAsBinary->children = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary*) * accessTree->numChildren);
    }

	for(int i = 0; i < accessTree->numChildren; i++)
	{
		accessTreeAsBinary->children[i] = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary));
		bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(accessTreeAsBinary->children[i], accessTree->children[i]);
	}
}