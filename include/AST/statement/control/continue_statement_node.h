#ifndef SYNTHSCRIPT_CONTINUESTATEMENTNODE_H
#define SYNTHSCRIPT_CONTINUESTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class ContinueStatementNode : public ASTNode {
public:
    ContinueStatementNode(int line, int col) : ASTNode(line, col) {}
    ~ContinueStatementNode() override = default;

    NodeType get_node_type() const override { return CONTINUE_STATEMENT_NODE; }
    static NodeType get_node_type_static() { return CONTINUE_STATEMENT_NODE; }

    DECLARE_VISITOR_FUNCTIONS;
};

#endif // SYNTHSCRIPT_CONTINUESTATEMENTNODE_H
