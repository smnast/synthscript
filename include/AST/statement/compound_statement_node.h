#ifndef SYNTHSCRIPT_COMPOUNDSTATEMENTNODE_H
#define SYNTHSCRIPT_COMPOUNDSTATEMENTNODE_H

#include "AST/AST_node.h"
#include <utility>
#include <vector>

class CompoundStatementNode : public ASTNode {
public:
    explicit CompoundStatementNode(std::vector<ASTNode*> statements, int line, int col) : ASTNode(line, col), statements(std::move(statements)) {}
    ~CompoundStatementNode() override {
        for (auto &statement : statements) {
            delete statement;
        }
    }
    std::vector<ASTNode*> *getStatements() { return &statements; }
    DECLARE_VISITOR_FUNCTIONS
private:
    std::vector<ASTNode*> statements;
};

#endif //SYNTHSCRIPT_COMPOUNDSTATEMENTNODE_H
