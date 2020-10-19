#include <stdlib.h>

#include "util/LogicalExpressionTree.h"
#include "util/LogicalExpressionTreeOperators.h"

int isLeaf(const CryptidLogicalExpressionTree *node) {
  return !node->leftChild && !node->rightChild;
}

int solveLogicalExpressionTree(CryptidLogicalExpressionTree *expressionTree) {
  if (!isLeaf(expressionTree)) {
    int branchValue;
    switch (*(CryptidLogicalExpressionTreeOperators *) expressionTree->value) {
    case AND:
      free(expressionTree->value);
      branchValue = (solveLogicalExpressionTree(expressionTree->leftChild) &&
                     solveLogicalExpressionTree(expressionTree->rightChild));
      expressionTree->value = &branchValue;
      break;
    case OR:
      free(expressionTree->value);
      branchValue = (solveLogicalExpressionTree(expressionTree->leftChild) ||
                     solveLogicalExpressionTree(expressionTree->rightChild));
      expressionTree->value = &branchValue;
      break;
    case NEG:
      free(expressionTree->value);
      branchValue = (!solveLogicalExpressionTree(expressionTree->rightChild));
      expressionTree->value = &branchValue;
      break;
    case IMPL:
      free(expressionTree->value);
      branchValue = (!solveLogicalExpressionTree(expressionTree->leftChild) ||
                     solveLogicalExpressionTree(expressionTree->rightChild));
      expressionTree->value = &branchValue;
      break;
    default:
      break;
    }
  }

  return *(int *)expressionTree->value;
}

void LogicalExpressionTree_destroy(CryptidLogicalExpressionTree *node) {
  if(node->leftChild) {
    LogicalExpressionTree_destroy(node->leftChild);
  }

  if(node->rightChild) {
    LogicalExpressionTree_destroy(node->rightChild);
  }

  free(node->value);
  free(node);
}