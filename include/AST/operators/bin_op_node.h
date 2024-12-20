#ifndef SYNTHSCRIPT_BINOPNODE_H
#define SYNTHSCRIPT_BINOPNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class BinOpNode : public ASTNode {
public:
    BinOpNode(TokenType op, ASTNode *left, ASTNode *right, int line, int col)
        : ASTNode(line, col), op(op), left(left), right(right) {}
    ~BinOpNode() override {
        delete left;
        delete right;
    }

    NodeType get_node_type() const override { return BIN_OP_NODE; }
    static NodeType get_node_type_static() { return BIN_OP_NODE; }

    TokenType get_op() { return op; }
    ASTNode *get_left_node() { return left; }
    ASTNode *get_right_node() { return right; }

    DECLARE_VISITOR_FUNCTIONS

private:
    TokenType op;
    ASTNode *left;
    ASTNode *right;
};

#endif // SYNTHSCRIPT_BINOPNODE_H
