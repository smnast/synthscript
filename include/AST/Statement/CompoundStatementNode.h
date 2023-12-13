#ifndef CLIKE_COMPOUNDSTATEMENTNODE_H
#define CLIKE_COMPOUNDSTATEMENTNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class CompoundStatementNode : public ASTNode {
public:
    explicit CompoundStatementNode(std::vector<ASTNode*> statements, int lineNumber) : ASTNode(lineNumber), statements(std::move(statements)) {}
    ~CompoundStatementNode() override {
        for (auto &statement : statements) {
            delete statement;
        }
    }
    std::vector<ASTNode*> *getStatements() { return &statements; }
private:
    std::vector<ASTNode*> statements;
};

#endif //CLIKE_COMPOUNDSTATEMENTNODE_H
