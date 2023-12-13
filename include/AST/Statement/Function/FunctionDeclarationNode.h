#ifndef CLIKE_FUNCTIONDECLARATIONNODE_H
#define CLIKE_FUNCTIONDECLARATIONNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(std::string identifier, std::vector<std::string> parameters, ASTNode *body, int line, int col) : ASTNode(line, col), identifier(std::move(identifier)), parameters(std::move(parameters)), body(body) {}
    ~FunctionDeclarationNode() override {
        delete body;
    }
    std::string getIdentifier() const { return identifier; }
    std::vector<std::string> *getParameters() { return &parameters; }
    ASTNode *getBody() { return body; }
private:
    std::string identifier;
    std::vector<std::string> parameters;
    ASTNode *body;
};

#endif //CLIKE_FUNCTIONDECLARATIONNODE_H
