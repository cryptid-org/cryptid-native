#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "greatest.h"

#include "util/LogicalExpressionTree.h"
#include "util/LogicalExpressionTreeOperators.h"

TEST logical_expression_tree_should_just_work() {
  // Given
  CryptidLogicalExpressionTree *root =
      malloc(sizeof(CryptidLogicalExpressionTree));
  root->value = malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)root->value = AND;

  root->leftChild = malloc(sizeof(CryptidLogicalExpressionTree));
  root->leftChild->value = malloc(sizeof(int));
  *(int *)root->leftChild->value = 1;

  root->rightChild = malloc(sizeof(CryptidLogicalExpressionTree));
  root->rightChild->value =
      malloc(sizeof(CryptidLogicalExpressionTreeOperators));
  *(CryptidLogicalExpressionTreeOperators *)root->rightChild->value = OR;

  root->rightChild->leftChild = malloc(sizeof(CryptidLogicalExpressionTree));
  root->rightChild->leftChild->value = malloc(sizeof(int));
  *(int *)root->rightChild->leftChild->value = 1;

  root->rightChild->rightChild = malloc(sizeof(CryptidLogicalExpressionTree));
  root->rightChild->rightChild->value = malloc(sizeof(int));
  *(int *)root->rightChild->rightChild->value = 1;

  // When
  int result;

  result = solveLogicalExpressionTree(root);

  // Then
  ASSERT(result == 1);

  PASS();
}

SUITE(cryptid_logical_expression_tree_suite) {
  { RUN_TESTp(logical_expression_tree_should_just_work); }
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(cryptid_logical_expression_tree_suite);

  GREATEST_MAIN_END();
}