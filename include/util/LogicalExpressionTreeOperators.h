#ifndef __CRYPTID_LOGICAL_EXPRESSION_TREE_OPERATORS_H
#define __CRYPTID_LOGICAL_EXPRESSION_TREE_OPERATORS_H

typedef enum {
  AND = 0,
  OR = 1,
  NEG = 2,
  IMPL = 3
} CryptidLogicalExpressionTreeOperators;

#endif