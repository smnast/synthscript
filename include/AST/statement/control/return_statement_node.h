#ifndef SYNTHSCRIPT_RETURNSTATEMENTNODE_H
#define SYNTHSCRIPT_RETURNSTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class ReturnStatementNode : public ASTNode {
public:
    ReturnStatementNode(ASTNode *value, int line, int col) : ASTNode(line, col), value(value) {}
    ~ReturnStatementNode() override { delete value; }

    bool has_value() { return value != nullptr; }
    ASTNode *get_value() { return value; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *value;
};

#endif // SYNTHSCRIPT_RETURNSTATEMENTNODE_H
