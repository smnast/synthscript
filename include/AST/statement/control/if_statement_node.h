#ifndef SYNTHSCRIPT_IFSTATEMENTNODE_H
#define SYNTHSCRIPT_IFSTATEMENTNODE_H

#include "AST/AST_node.h"

class IfStatementNode : public ASTNode {
public:
    IfStatementNode(ASTNode *condition, ASTNode *if_body, ASTNode *else_body, int line, int col)
        : ASTNode(line, col), condition(condition), if_body(if_body), else_body(else_body) {}

    ~IfStatementNode() override {
        delete condition;
        delete if_body;
        delete else_body;
    }

    ASTNode *get_condition() { return condition; }
    ASTNode *get_if_body() { return if_body; }
    ASTNode *get_else_body() { return else_body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *condition;
    ASTNode *if_body;
    ASTNode *else_body;
};

#endif //SYNTHSCRIPT_IFSTATEMENTNODE_H
