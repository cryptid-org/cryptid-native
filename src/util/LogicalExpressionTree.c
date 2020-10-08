#include <stdlib.h>

#include "util/LogicalExpressionTree.h"
#include "util/LogicalExpressionTreeOperators.h"

int isLeaf(CryptidLogicalExpressionTree* node) {
    return !node->leftChild && !node->rightChild;
}

int solveLogicalExpressionTree(CryptidLogicalExpressionTree* expressionTree) {
    if(!isLeaf(expressionTree)) {
        int branchValue;
        CryptidLogicalExpressionTreeOperators operator = *(CryptidLogicalExpressionTreeOperators*)expressionTree->value;
        switch (operator) {
            case AND:
                free(expressionTree->value);
                branchValue = (solveLogicalExpressionTree(expressionTree->leftChild) && solveLogicalExpressionTree(expressionTree->rightChild));
                expressionTree->value = &branchValue;
                break;
            case OR:
                free(expressionTree->value);
                branchValue = (solveLogicalExpressionTree(expressionTree->leftChild) || solveLogicalExpressionTree(expressionTree->rightChild));
                expressionTree->value = &branchValue;
                break;
            case NEG:
                free(expressionTree->value);
                branchValue = (!solveLogicalExpressionTree(expressionTree->rightChild));
                expressionTree->value = &branchValue;
                break;
            case IMPL:
                free(expressionTree->value);
                branchValue = (!solveLogicalExpressionTree(expressionTree->leftChild) || solveLogicalExpressionTree(expressionTree->rightChild));
                expressionTree->value = &branchValue;
                break;
            default:
                break;
        }
    }

    return *(int*)expressionTree->value;
}