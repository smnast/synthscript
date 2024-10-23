#ifndef SYNTHSCRIPT_CASTOPNODE_H
#define SYNTHSCRIPT_CASTOPNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"
#include <utility>

class CastOpNode : public ASTNode {
public:
    CastOpNode(Type type, ASTNode *operand, int line, int col)
        : ASTNode(line, col), type(std::move(type)), operand(operand) {}
    ~CastOpNode() override { delete operand; }

    NodeType get_node_type() const override { return CAST_OP_NODE; }

    Type get_type() const { return type; }
    ASTNode *get_operand() const { return operand; }

    DECLARE_VISITOR_FUNCTIONS

private:
    Type type;
    ASTNode *operand;
};

#endif // SYNTHSCRIPT_CASTOPNODE_H
