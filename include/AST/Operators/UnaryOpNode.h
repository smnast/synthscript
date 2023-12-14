#ifndef CLIKE_UNARYOPNODE_H
#define CLIKE_UNARYOPNODE_H

#include <functional>
#include "AST/ASTNode.h"

class UnaryOpNode : public ASTNode {
public:
    UnaryOpNode(TokenType op, ASTNode *operand, int line, int col) : ASTNode(line, col), op(op), operand(operand) {}
    ~UnaryOpNode() override {
        delete operand;
    }
    TokenType getOp() { return op; }
    ASTNode *getOperand() { return operand; }
    DECLARE_VISITOR_FUNCTIONS
private:
    TokenType op;
    ASTNode *operand;
};

#endif //CLIKE_UNARYOPNODE_H
