#ifndef SYNTHSCRIPT_CONTINUESTATEMENTNODE_H
#define SYNTHSCRIPT_CONTINUESTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class ContinueStatementNode : public ASTNode {
public:
    ContinueStatementNode(int line, int col) : ASTNode(line, col) {}
    ~ContinueStatementNode() override = default;

    DECLARE_VISITOR_FUNCTIONS;
};

#endif // SYNTHSCRIPT_CONTINUESTATEMENTNODE_H
