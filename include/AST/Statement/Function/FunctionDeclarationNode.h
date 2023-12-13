#ifndef CLIKE_FUNCTIONDECLARATIONNODE_H
#define CLIKE_FUNCTIONDECLARATIONNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(Type returnType, ASTNode *identifier, std::vector<std::pair<Type, ASTNode*>> parameters, ASTNode *body, int lineNumber) : ASTNode(lineNumber), returnType(returnType), identifier(identifier), parameters(std::move(parameters)), body(body) {}
    ~FunctionDeclarationNode() override {
        for (auto &parameter : parameters) {
            delete parameter.second;
        }
        delete identifier;
        delete body;
    }
    Type getReturnType() { return returnType; }
    ASTNode *getIdentifier() { return identifier; }
    std::vector<std::pair<Type, ASTNode*>> *getParameters() { return &parameters; }
    ASTNode *getBody() { return body; }
private:
    Type returnType;
    ASTNode *identifier;
    std::vector<std::pair<Type, ASTNode*>> parameters;
    ASTNode *body;
};

#endif //CLIKE_FUNCTIONDECLARATIONNODE_H
