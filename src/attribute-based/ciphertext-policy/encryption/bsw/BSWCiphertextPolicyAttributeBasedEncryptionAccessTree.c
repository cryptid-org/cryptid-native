#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"

// Creating an access tree as specified in Bethencourt-Sahai-Waters CP-ABE pdf
// Value is threshold
//	value = 1 meaning an OR gate
//	value = [number of childrens] meaning an AND gate
BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_init(const int value, char* attribute, const size_t attributeLength, const int numChildren)
{
	BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* tree = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree));
    tree->value = value;
    tree->computed = 0;
    if(numChildren > 0)
    {
        tree->children = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree*) * numChildren);
    }
    tree->numChildren = numChildren;

    tree->attribute = attribute;
    tree->attributeLength = attributeLength;

    return tree;
}

// Returning whether a node of a tree is leaf
int BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree)
{
	return (accessTree->numChildren == 0) ? 1 : 0;
}

// Returning 1 if attributes satisfy the accessTree, else 0
int BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, char** attributes, const int numAttributes)
{
	if(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(accessTree))
    {
		return BSWCiphertextPolicyAttributeBasedEncryptionHasAttribute(attributes, numAttributes, accessTree->attribute);
	}
    
	int i;
	int counter = 0;
	for(i = 0; i < accessTree->numChildren; i++)
    {
		if(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(accessTree->children[i], attributes, numAttributes))
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

// Calculates cY and cY' (cYa) values for accessTree and its children recursively (y ∈ leaf nodes)
CryptidStatus BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree, const mpz_t s, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey)
{
    if(!BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(accessTree))
    {
        int d = accessTree->value - 1; // dx = kx-1, degree = threshold-1
        BSWCiphertextPolicyAttributeBasedEncryptionPolynom* q = BSWCiphertextPolicyAttributeBasedEncryptionPolynom_init(d, s, publickey);

        int i;
        for(i = 0; i < accessTree->numChildren; i++)
        {
            mpz_t sum;
            mpz_init(sum);
            BSWCiphertextPolicyAttributeBasedEncryptionPolynomSum(q, i+1, sum);
            BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(accessTree->children[i], sum, publickey);
            mpz_clear(sum);
        }

        BSWCiphertextPolicyAttributeBasedEncryptionPolynom_destroy(q);
    }
    else
    {
        AffinePoint cY;
        CryptidStatus status = affine_wNAFMultiply(&cY, publickey->g, s, publickey->ellipticCurve);
        if(status)
        {
            affine_destroy(cY);
            return status;
        }

        // H(att(x))
        AffinePoint hashedPoint;

        status = hashToPoint(&hashedPoint, accessTree->attribute, accessTree->attributeLength, publickey->q, publickey->ellipticCurve, publickey->hashFunction);

        if (status) 
        {
            return status;
        }

        AffinePoint cYa;
        status = affine_wNAFMultiply(&cYa, hashedPoint, s, publickey->ellipticCurve);
        if(status)
        {
            return status;
        }

        affine_destroy(hashedPoint);

        accessTree->cY = cY;
        accessTree->cYa = cYa;
        accessTree->computed = 1;
    }

    return CRYPTID_SUCCESS;
}

// Used for deleting the tree and its children from memory
void BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* tree)
{
	for(int i = 0; i < tree->numChildren; i++)
    {
        BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(tree->children[i]);
    }
    if(tree->computed)
    {
    	affine_destroy(tree->cY);
    	affine_destroy(tree->cYa);
    }
    if(tree->numChildren > 0)
    {
        free(tree->children);
    }
    free(tree);
}