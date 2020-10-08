#ifndef __CRYPTID_EXPRESSION_TREE_H
#define __CRYPTID_EXPRESSION_TREE_H

typedef struct CryptidLogicalExpressionTree {
  void* value;

  CryptidLogicalExpressionTree* leftChild;

  CryptidLogicalExpressionTree* rightChild;

} CryptidLogicalExpressionTree;

void buildLogicalExpressionTreeFromString(CryptidLogicalExpressionTree* result, char* input, size_t inputLength);

boolean isLeaf(CryptidLogicalExpressionTree node);

boolean solveLogicalExpressionTree(CryptidExpressionTree* expressionTree);

#endif
