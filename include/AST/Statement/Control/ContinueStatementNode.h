#ifndef CLIKE_CONTINUESTATEMENTNODE_H
#define CLIKE_CONTINUESTATEMENTNODE_H

#include "AST/ASTNode.h"

class ContinueStatementNode : public ASTNode {
public:
    explicit ContinueStatementNode(int lineNumber) : ASTNode(lineNumber) {}
    ~ContinueStatementNode() override = default;
};

#endif //CLIKE_CONTINUESTATEMENTNODE_H
