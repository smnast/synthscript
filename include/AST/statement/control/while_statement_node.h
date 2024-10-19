#ifndef SYNTHSCRIPT_WHILESTATEMENTNODE_H
#define SYNTHSCRIPT_WHILESTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class WhileStatementNode : public ASTNode {
public:
    WhileStatementNode(ASTNode *condition, ASTNode *body, int line, int col)
        : ASTNode(line, col), condition(condition), body(body) {}
    ~WhileStatementNode() override {
        delete condition;
        delete body;
    }

    ASTNode *get_condition() { return condition; }
    ASTNode *get_body() { return body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *condition;
    ASTNode *body;
};

#endif // SYNTHSCRIPT_WHILESTATEMENTNODE_H
