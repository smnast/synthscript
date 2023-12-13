#ifndef CLIKE_COMPOUNDSTATEMENTNODE_H
#define CLIKE_COMPOUNDSTATEMENTNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class CompoundStatementNode : public ASTNode {
public:
    explicit CompoundStatementNode(std::vector<ASTNode*> statements, int line, int col) : ASTNode(line, col), statements(std::move(statements)) {}
    ~CompoundStatementNode() override = default;
    std::vector<ASTNode*> *getStatements() { return &statements; }
private:
    std::vector<ASTNode*> statements;
};

#endif //CLIKE_COMPOUNDSTATEMENTNODE_H
