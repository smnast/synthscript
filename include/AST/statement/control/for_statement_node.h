#ifndef SYNTHSCRIPT_FORSTATEMENTNODE_H
#define SYNTHSCRIPT_FORSTATEMENTNODE_H

#include "AST/AST_node.h"
#include <utility>

class ForStatementNode : public ASTNode {
public:
    ForStatementNode(std::string identifier, ASTNode *iterable, ASTNode *body, int line, int col)
        : ASTNode(line, col), identifier(std::move(identifier)), iterable(iterable), body(body) {}

    ~ForStatementNode() override {
        delete iterable;
        delete body;
    }

    std::string get_identifier() const { return identifier; }
    ASTNode *get_iterable() const { return iterable; }
    ASTNode *get_body() const { return body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::string identifier;
    ASTNode *iterable;
    ASTNode *body;
};

#endif //SYNTHSCRIPT_FORSTATEMENTNODE_H
