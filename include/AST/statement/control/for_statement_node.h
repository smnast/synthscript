#ifndef SYNTHSCRIPT_FORSTATEMENTNODE_H
#define SYNTHSCRIPT_FORSTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"
#include <utility>

class ForStatementNode : public ASTNode {
public:
    ForStatementNode(std::string identifier, ASTNode *iterable, ASTNode *body, int line, int col)
        : ASTNode(line, col), identifier(std::move(identifier)), iterable(iterable), body(body) {}
    ~ForStatementNode() override {
        delete iterable;
        delete body;
    }

    NodeType get_node_type() const override { return FOR_STATEMENT_NODE; }
    static NodeType get_node_type_static() { return FOR_STATEMENT_NODE; }

    std::string get_identifier() const { return identifier; }
    ASTNode *get_iterable() const { return iterable; }
    ASTNode *get_body() const { return body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::string identifier;
    ASTNode *iterable;
    ASTNode *body;
};

#endif // SYNTHSCRIPT_FORSTATEMENTNODE_H
