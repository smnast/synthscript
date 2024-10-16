#ifndef SYNTHSCRIPT_CONTINUESTATEMENTNODE_H
#define SYNTHSCRIPT_CONTINUESTATEMENTNODE_H

#include "AST/AST_node.h"

class ContinueStatementNode : public ASTNode {
public:
    explicit ContinueStatementNode(int line, int col) : ASTNode(line, col) {}
    ~ContinueStatementNode() override = default;

    DECLARE_VISITOR_FUNCTIONS;
};

#endif // SYNTHSCRIPT_CONTINUESTATEMENTNODE_H
