#ifndef CLIKE_BINOPNODE_H
#define CLIKE_BINOPNODE_H

#include "AST/ASTNode.h"

class BinOpNode : public ASTNode {
public:
    BinOpNode(std::string op, ASTNode *left, ASTNode *right, int line, int col) : ASTNode(line, col), op(std::move(op)), left(left), right(right) {}
    ~BinOpNode() override {
        delete left;
        delete right;
    }
    std::string getOp() { return op; }
    ASTNode *getLeftNode() { return left; }
    ASTNode *getRightNode() { return right; }
private:
    std::string op;
    ASTNode *left;
    ASTNode *right;
};

#endif //CLIKE_BINOPNODE_H
