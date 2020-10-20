#include <stdlib.h>

#include "util/LogicalExpressionTree.h"
#include "util/LogicalExpressionTreeOperators.h"

int isLeaf(const CryptidLogicalExpressionTree *node) {
  return !node->leftChild && !node->rightChild;
}

int solveLogicalExpressionTree(
    const CryptidLogicalExpressionTree *expressionTree) {
  if (!isLeaf(expressionTree)) {
    int branchValue;
    switch (*(CryptidLogicalExpressionTreeOperators *)expressionTree->value) {
    case AND:
      branchValue = (solveLogicalExpressionTree(expressionTree->leftChild) &&
                     solveLogicalExpressionTree(expressionTree->rightChild));
      break;
    case OR:
      branchValue = (solveLogicalExpressionTree(expressionTree->leftChild) ||
                     solveLogicalExpressionTree(expressionTree->rightChild));
      break;
    case NEG:
      branchValue = (!solveLogicalExpressionTree(expressionTree->rightChild));
      break;
    case IMPL:
      branchValue = (!solveLogicalExpressionTree(expressionTree->leftChild) ||
                     solveLogicalExpressionTree(expressionTree->rightChild));
      break;
    }

    return branchValue;
  }

  return *(int *)expressionTree->value;
}

void LogicalExpressionTree_destroy(CryptidLogicalExpressionTree *node) {
  if (node->leftChild) {
    LogicalExpressionTree_destroy(node->leftChild);
  }

  if (node->rightChild) {
    LogicalExpressionTree_destroy(node->rightChild);
  }

  free(node->value);
  free(node);
}