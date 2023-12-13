#ifndef CLIKE_ARRAYDECLARATIONSTATEMENTNODE_H
#define CLIKE_ARRAYDECLARATIONSTATEMENTNODE_H

#include <utility>
#include <vector>
#include <utility>

#include "AST/ASTNode.h"

class ArrayDeclarationStatementNode : public ASTNode {
public:
    ArrayDeclarationStatementNode(Type type, ASTNode *identifier, std::vector<ASTNode*> dimensions, int lineNumber) : ASTNode(lineNumber), type(type), identifier(identifier), dimensions(std::move(dimensions)) {}
    ~ArrayDeclarationStatementNode() override {
        delete identifier;
        for (auto &dimension : dimensions) {
            delete dimension;
        }
    }
    Type getType() { return type; }
    ASTNode *getIdentifier() { return identifier; }
    std::vector<ASTNode*> *getDimensions() { return &dimensions; }
private:
    Type type;
    ASTNode *identifier;
    std::vector<ASTNode*> dimensions;
};

#endif //CLIKE_ARRAYDECLARATIONSTATEMENTNODE_H
