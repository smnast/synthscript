#ifndef SYNTHSCRIPT_PROGRAMNODE_H
#define SYNTHSCRIPT_PROGRAMNODE_H

#include "AST_node.h"
#include <utility>
#include <vector>

class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(std::vector<ASTNode *> statements, int line, int col)
        : ASTNode(line, col), statements(std::move(statements)) {}

    ~ProgramNode() override {
        for (auto &statement : statements) {
            delete statement;
        }
    }

    std::vector<ASTNode *> *get_statements() { return &statements; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::vector<ASTNode *> statements;
};

#endif // SYNTHSCRIPT_PROGRAMNODE_H
