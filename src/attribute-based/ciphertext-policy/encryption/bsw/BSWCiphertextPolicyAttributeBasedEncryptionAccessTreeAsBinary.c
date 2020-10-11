#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary.h"

bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *
bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_init(
    const int value, char *attribute, const size_t attributeLength,
    const int numChildren) {
  bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *tree = (bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *)malloc(
      sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary));

  tree->value = value;

  tree->computed = 0;

  if (numChildren > 0) {
    tree->children =
        (bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary **)malloc(sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *) *
               numChildren);
  }

  tree->numChildren = numChildren;

  tree->attributeLength = attributeLength;
  if (attributeLength > 0) {
    tree->attribute = malloc(attributeLength + 1);
    memcpy(tree->attribute, attribute, attributeLength + 1);
  }

  return tree;
}

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
        *accessTreeAsBinary) {
  if (accessTreeAsBinary->computed) {
    affineAsBinary_destroy(accessTreeAsBinary->cY);
    affineAsBinary_destroy(accessTreeAsBinary->cYa);
  }

  int numChildren = accessTreeAsBinary->numChildren;
  for (int i = 0; i < numChildren; i++) {
    bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(
        accessTreeAsBinary->children[i]);
  }
  if (numChildren > 0) {
    free(accessTreeAsBinary->children);
  }

  if (accessTreeAsBinary->attributeLength > 0) {
    free(accessTreeAsBinary->attribute);
  }

  free(accessTreeAsBinary);
}

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree,
    const bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
        *accessTreeAsBinary) {
  accessTree->value = accessTreeAsBinary->value;

  accessTree->computed = accessTreeAsBinary->computed;

  accessTree->numChildren = accessTreeAsBinary->numChildren;

  accessTree->attributeLength = accessTreeAsBinary->attributeLength;

  if (accessTree->computed) {
    affineAsBinary_toAffine(&accessTree->cY, accessTreeAsBinary->cY);
    affineAsBinary_toAffine(&accessTree->cYa, accessTreeAsBinary->cYa);
  }

  if (accessTree->attributeLength > 0) {
    accessTree->attribute = (char *)malloc(accessTree->attributeLength + 1);
    memcpy(accessTree->attribute, accessTreeAsBinary->attribute,
           accessTree->attributeLength + 1);
  } else {
    accessTree->attribute = NULL;
  }

  if (accessTree->numChildren > 0) {
    accessTree->children =
        (bswCiphertextPolicyAttributeBasedEncryptionAccessTree **)malloc(sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTree *) *
               accessTree->numChildren);
  }

  for (int i = 0; i < accessTree->numChildren; i++) {
    accessTree->children[i] =
        (bswCiphertextPolicyAttributeBasedEncryptionAccessTree *)malloc(sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTree));
    bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
        accessTree->children[i], accessTreeAsBinary->children[i]);
  }
}

void bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
    bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary
        *accessTreeAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionAccessTree *accessTree) {
  accessTreeAsBinary->value = accessTree->value;

  accessTreeAsBinary->computed = accessTree->computed;

  accessTreeAsBinary->numChildren = accessTree->numChildren;

  accessTreeAsBinary->attributeLength = accessTree->attributeLength;

  if (accessTree->computed) {
    affineAsBinary_fromAffine(&accessTreeAsBinary->cY, accessTree->cY);
    affineAsBinary_fromAffine(&accessTreeAsBinary->cYa, accessTree->cYa);
  }

  if (accessTree->attributeLength > 0) {
    accessTreeAsBinary->attribute = malloc(accessTree->attributeLength + 1);
    memcpy(accessTreeAsBinary->attribute, accessTree->attribute,
           accessTree->attributeLength + 1);
  }

  if (accessTree->numChildren > 0) {
    accessTreeAsBinary->children = (bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary **)malloc(
        sizeof(
            bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *) *
        accessTree->numChildren);
  }

  for (int i = 0; i < accessTree->numChildren; i++) {
    accessTreeAsBinary->children[i] = (bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *)malloc(
        sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary));
    bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(accessTreeAsBinary->children[i], accessTree->children[i]);
  }
}