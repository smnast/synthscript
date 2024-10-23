#ifndef SYNTHSCRIPT_SUBSCRIPTOPNODE_H
#define SYNTHSCRIPT_SUBSCRIPTOPNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class SubscriptOpNode : public ASTNode {
public:
    SubscriptOpNode(ASTNode *identifier, ASTNode *index, int line, int col)
        : ASTNode(line, col), identifier(identifier), index(index) {}
    ~SubscriptOpNode() override {
        delete identifier;
        delete index;
    }

    NodeType get_node_type() const override { return SUBSCRIPT_OP_NODE; }
    static NodeType get_node_type_static() { return SUBSCRIPT_OP_NODE; }

    ASTNode *get_identifier() { return identifier; }
    ASTNode *get_index() { return index; }
    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *identifier;
    ASTNode *index;
};

#endif // SYNTHSCRIPT_SUBSCRIPTOPNODE_H
