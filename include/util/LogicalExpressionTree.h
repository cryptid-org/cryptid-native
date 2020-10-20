#ifndef __CRYPTID_EXPRESSION_TREE_H
#define __CRYPTID_EXPRESSION_TREE_H

typedef struct CryptidLogicalExpressionTree CryptidLogicalExpressionTree;

struct CryptidLogicalExpressionTree {
  void *value;

  CryptidLogicalExpressionTree *leftChild;

  CryptidLogicalExpressionTree *rightChild;
};

int isLeaf(const CryptidLogicalExpressionTree *node);

int solveLogicalExpressionTree(
    const CryptidLogicalExpressionTree *expressionTree);

void LogicalExpressionTree_destroy(CryptidLogicalExpressionTree *node);

#endif
