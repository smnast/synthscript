#ifndef SYNTHSCRIPT_COMPOUNDSTATEMENTNODE_H
#define SYNTHSCRIPT_COMPOUNDSTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"
#include <utility>
#include <vector>

class CompoundStatementNode : public ASTNode {
public:
    CompoundStatementNode(std::vector<ASTNode *> statements, int line, int col)
        : ASTNode(line, col), statements(std::move(statements)) {}
    ~CompoundStatementNode() override {
        for (auto &statement : statements) {
            delete statement;
        }
    }

    NodeType get_node_type() const override { return COMPOUND_STATEMENT_NODE; }
    static NodeType get_node_type_static() { return COMPOUND_STATEMENT_NODE; }

    std::vector<ASTNode *> *get_statements() { return &statements; }
    size_t get_statements_size() { return statements.size(); }
    ASTNode *get_statement(size_t index) { return statements[index]; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::vector<ASTNode *> statements;
};

#endif // SYNTHSCRIPT_COMPOUNDSTATEMENTNODE_H
