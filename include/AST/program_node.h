#ifndef SYNTHSCRIPT_PROGRAMNODE_H
#define SYNTHSCRIPT_PROGRAMNODE_H

#include "AST/visit_functions_macro.h"
#include "AST_node.h"
#include <utility>
#include <vector>

class ProgramNode : public ASTNode {
public:
    ProgramNode(std::vector<ASTNode *> statements, int line, int col)
        : ASTNode(line, col), statements(std::move(statements)) {}
    ~ProgramNode() override {
        for (auto &statement : statements) {
            delete statement;
        }
    }

    NodeType get_node_type() const override { return PROGRAM_NODE; }
    static NodeType get_node_type_static() { return PROGRAM_NODE; }

    std::vector<ASTNode *> *get_statements() { return &statements; }
    size_t get_statements_size() { return statements.size(); }
    ASTNode *get_statement(size_t index) { return statements[index]; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::vector<ASTNode *> statements;
};

#endif // SYNTHSCRIPT_PROGRAMNODE_H
