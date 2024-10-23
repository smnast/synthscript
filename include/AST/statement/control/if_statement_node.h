#ifndef SYNTHSCRIPT_IFSTATEMENTNODE_H
#define SYNTHSCRIPT_IFSTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class IfStatementNode : public ASTNode {
public:
    IfStatementNode(ASTNode *condition, ASTNode *if_body, ASTNode *else_body, int line, int col)
        : ASTNode(line, col), condition(condition), if_body(if_body), else_body(else_body) {}
    ~IfStatementNode() override {
        delete condition;
        delete if_body;
        delete else_body;
    }
    
    NodeType get_node_type() const override { return IF_STATEMENT_NODE; }

    ASTNode *get_condition() { return condition; }
    ASTNode *get_if_body() { return if_body; }
    
    bool has_else_body() { return else_body != nullptr; }
    ASTNode *get_else_body() { return else_body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *condition;
    ASTNode *if_body;
    ASTNode *else_body;
};

#endif // SYNTHSCRIPT_IFSTATEMENTNODE_H
