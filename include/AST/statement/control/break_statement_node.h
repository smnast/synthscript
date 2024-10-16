#ifndef SYNTHSCRIPT_BREAKSTATEMENTNODE_H
#define SYNTHSCRIPT_BREAKSTATEMENTNODE_H

#include "AST/AST_node.h"

class BreakStatementNode : public ASTNode {
public:
    explicit BreakStatementNode(int line, int col) : ASTNode(line, col) {}
    ~BreakStatementNode() override = default;

    DECLARE_VISITOR_FUNCTIONS
};

#endif // SYNTHSCRIPT_BREAKSTATEMENTNODE_H
