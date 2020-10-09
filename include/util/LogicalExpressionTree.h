#ifndef __CRYPTID_EXPRESSION_TREE_H
#define __CRYPTID_EXPRESSION_TREE_H

typedef struct CryptidLogicalExpressionTree CryptidLogicalExpressionTree;

struct CryptidLogicalExpressionTree {
  void* value;

  CryptidLogicalExpressionTree* leftChild;

  CryptidLogicalExpressionTree* rightChild;

  CryptidLogicalExpressionTree* parent;

};

int isLeaf(const CryptidLogicalExpressionTree* node);

int solveLogicalExpressionTree(CryptidLogicalExpressionTree* expressionTree);

#endif
