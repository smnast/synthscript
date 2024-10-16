#ifndef SYNTHSCRIPT_FUNCTIONDECLARATIONNODE_H
#define SYNTHSCRIPT_FUNCTIONDECLARATIONNODE_H

#include "AST/AST_node.h"
#include <utility>
#include <vector>

class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(std::string identifier, std::vector<std::string> parameters, ASTNode *body, int line, int col) : ASTNode(line, col), identifier(std::move(identifier)), parameters(std::move(parameters)), body(body) {}

    ~FunctionDeclarationNode() override { delete body; }

    std::string get_identifier() const { return identifier; }
    std::vector<std::string> *get_parameters() { return &parameters; }
    ASTNode *get_body() { return body; }
    DECLARE_VISITOR_FUNCTIONS

private:
    std::string identifier;
    std::vector<std::string> parameters;
    ASTNode *body;
};

#endif // SYNTHSCRIPT_FUNCTIONDECLARATIONNODE_H
