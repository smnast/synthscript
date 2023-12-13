#ifndef CLIKE_BREAKSTATEMENTNODE_H
#define CLIKE_BREAKSTATEMENTNODE_H

#include "AST/ASTNode.h"

class BreakStatementNode : public ASTNode {
public:
    explicit BreakStatementNode(int lineNumber) : ASTNode(lineNumber) {}
    ~BreakStatementNode() override = default;
};

#endif //CLIKE_BREAKSTATEMENTNODE_H
