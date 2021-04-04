#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTree.h"

// Creating an access tree as specified in Bethencourt-Sahai-Waters CP-ABE pdf
// Value is threshold
//	value = 1 meaning an OR gate
//	value = [number of childrens] meaning an AND gate
bswCiphertextPolicyAttributeBasedEncryptionAccessTree *
bswCiphertextPolicyAttributeBasedEncryptionAccessTree_init(
    const int value, char *attribute, const size_t attributeLength,
    const int numChildren) {
  bswCiphertextPolicyAttributeBasedEncryptionAccessTree *tree =
      (bswCiphertextPolicyAttributeBasedEncryptionAccessTree *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTree));
  tree->value = value;
  tree->computed = 0;
  if (numChildren > 0) {
    tree->children =
        (bswCiphertextPolicyAttributeBasedEncryptionAccessTree **)malloc(
            sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTree *) *
            numChildren);
  }
  tree->numChildren = numChildren;

  tree->attribute = attribute;
  tree->attributeLength = attributeLength;

  return tree;
}

// Returning whether a node of a tree is leaf
int bswCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(
    const bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree) {
  return (accessTree->numChildren == 0) ? 1 : 0;
}

// Returning 1 if attributes satisfy the accessTree, else 0
int bswCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(
    const bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree,
    char **attributes, const int numAttributes) {
  if (bswCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(
          accessTree)) {
    return bswCiphertextPolicyAttributeBasedEncryptionHasAttribute(
        attributes, numAttributes, accessTree->attribute);
  }

  int i;
  int counter = 0;
  for (i = 0; i < accessTree->numChildren; i++) {
    if (bswCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(
            accessTree->children[i], attributes, numAttributes)) {
      counter++;
      if (counter >= accessTree->value) {
        return 1;
      }
    }
  }
  return 0;
}

// Calculates cY and cY' (cYa) values for accessTree and its children
// recursively (y ∈ leaf nodes)
CryptidStatus bswCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree,
    const mpz_t s,
    const bswCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey) {
  if (!bswCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(
          accessTree)) {
    int d = accessTree->value - 1; // dx = kx-1, degree = threshold-1
    bswCiphertextPolicyAttributeBasedEncryptionPolynom *q =
        bswCiphertextPolicyAttributeBasedEncryptionPolynom_init(d, s,
                                                                publickey);

    int i;
    for (i = 0; i < accessTree->numChildren; i++) {
      mpz_t sum;
      mpz_init(sum);
      bswCiphertextPolicyAttributeBasedEncryptionPolynomSum(q, i + 1, sum);
      bswCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(
          accessTree->children[i], sum, publickey);
      mpz_clear(sum);
    }

    bswCiphertextPolicyAttributeBasedEncryptionPolynom_destroy(q);
  } else {
    AffinePoint cY;
    CryptidStatus status =
        affine_wNAFMultiply(&cY, publickey->g, s, publickey->ellipticCurve);
    if (status) {
      affine_destroy(cY);
      return status;
    }

    // H(att(x))
    AffinePoint hashedPoint;

    status = hashToPoint(&hashedPoint, accessTree->attribute,
                         accessTree->attributeLength, publickey->q,
                         publickey->ellipticCurve, publickey->hashFunction);

    if (status) {
      return status;
    }

    AffinePoint cYa;
    status =
        affine_wNAFMultiply(&cYa, hashedPoint, s, publickey->ellipticCurve);
    if (status) {
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
void bswCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTree *tree) {
  for (int i = 0; i < tree->numChildren; i++) {
    bswCiphertextPolicyAttributeBasedEncryptionAccessTree_destroy(
        tree->children[i]);
  }
  if (tree->computed) {
    affine_destroy(tree->cY);
    affine_destroy(tree->cYa);
  }
  if (tree->numChildren > 0) {
    free(tree->children);
  } else {
    free(tree->attribute);
  }
  free(tree);
}