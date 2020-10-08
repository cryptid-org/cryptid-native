#ifndef __CRYPTID_EXPRESSION_TREE_LOGICAL_OPERATORS_H
#define __CRYPTID_EXPRESSION_TREE_LOGICAL_OPERATORS_H

#include "util/ExpressionTree.h"

typedef enum { 
    AND,
    OR,
    NEG,
    IMPL
    } CryptidExpressionTreeOperators;

    boolean solveLogicalExpressionTree(CryptidExpressionTree* expressionTree);

#endif