#ifndef CLIKE_BREAKSTATEMENTNODE_H
#define CLIKE_BREAKSTATEMENTNODE_H

#include "AST/ASTNode.h"

class BreakStatementNode : public ASTNode {
public:
    explicit BreakStatementNode(int line, int col) : ASTNode(line, col) {}
    ~BreakStatementNode() override = default;
    DECLARE_VISITOR_FUNCTIONS
};

#endif //CLIKE_BREAKSTATEMENTNODE_H
