#ifndef CLIKE_UNARYOPNODE_H
#define CLIKE_UNARYOPNODE_H

#include <functional>
#include "AST/ASTNode.h"

class UnaryOpNode : public ASTNode {
public:
    UnaryOpNode(std::string op, ASTNode *operand, int lineNumber) : ASTNode(lineNumber), op(std::move(op)), operand(operand) {}
    ~UnaryOpNode() override {
        delete operand;
    }
    std::string getOp() { return op; }
    ASTNode *getOperand() { return operand; }
private:
    std::string op;
    ASTNode *operand;
};

#endif //CLIKE_UNARYOPNODE_H
