#ifndef CLIKE_FUNCTIONDECLARATIONNODE_H
#define CLIKE_FUNCTIONDECLARATIONNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(ASTNode *identifier, std::vector<ASTNode*> parameters, ASTNode *body, int line, int col) : ASTNode(line, col), returnType(returnType), identifier(identifier), parameters(std::move(parameters)), body(body) {}
    ~FunctionDeclarationNode() override {
        delete identifier;
        delete body;
    }
    ASTNode *getIdentifier() { return identifier; }
    std::vector<ASTNode*> *getParameters() { return &parameters; }
    ASTNode *getBody() { return body; }
private:
    ASTNode *identifier;
    std::vector<ASTNode*> parameters;
    ASTNode *body;
};

#endif //CLIKE_FUNCTIONDECLARATIONNODE_H
