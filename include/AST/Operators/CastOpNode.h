#ifndef CLIKE_CASTOPNODE_H
#define CLIKE_CASTOPNODE_H

#include <utility>

#include "AST/ASTNode.h"

class CastOpNode : public ASTNode {
public:
    CastOpNode(Type type, ASTNode *operand, int lineNumber) : ASTNode(lineNumber), type(type), operand(operand) {}
    ~CastOpNode() override {
        delete operand;
    }
    Type getType() { return type; }
    ASTNode *getOperand() { return operand; }
private:
    Type type;
    ASTNode *operand;
};

#endif //CLIKE_CASTOPNODE_H
