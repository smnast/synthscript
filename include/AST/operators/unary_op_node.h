#ifndef SYNTHSCRIPT_UNARYOPNODE_H
#define SYNTHSCRIPT_UNARYOPNODE_H

#include "AST/AST_node.h"
#include <functional>

class UnaryOpNode : public ASTNode {
public:
    UnaryOpNode(TokenType op, ASTNode *operand, int line, int col) 
        : ASTNode(line, col), op(op), operand(operand) {}

    ~UnaryOpNode() override {
        delete operand;
    }

    TokenType get_op() { return op; }
    ASTNode *get_operand() { return operand; }

    DECLARE_VISITOR_FUNCTIONS

private:
    TokenType op;
    ASTNode *operand;
};

#endif //SYNTHSCRIPT_UNARYOPNODE_H
