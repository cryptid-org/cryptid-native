#ifndef __CRYPTID_EXPRESSION_TREE_H
#define __CRYPTID_EXPRESSION_TREE_H

typedef struct CryptidExpressionTree {
  void* value;

  CryptidExpressionTree* leftChild;

  CryptidExpressionTree* rightChild;

} CryptidExpressionTree;

void buildTreeFromString(CryptidExpressionTree* result, char* input, size_t inputLength);

boolean isLeaf(CryptidExpressionTree node);

#endif
