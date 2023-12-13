#ifndef CLIKE_BINOPNODE_H
#define CLIKE_BINOPNODE_H

#include "AST/ASTNode.h"

class BinOpNode : public ASTNode {
public:
    BinOpNode(TokenType op, ASTNode *left, ASTNode *right, int line, int col) : ASTNode(line, col), op(op), left(left), right(right) {}
    ~BinOpNode() override {
        delete left;
        delete right;
    }
    TokenType getOp() { return op; }
    ASTNode *getLeftNode() { return left; }
    ASTNode *getRightNode() { return right; }
private:
    TokenType op;
    ASTNode *left;
    ASTNode *right;
};

#endif //CLIKE_BINOPNODE_H
