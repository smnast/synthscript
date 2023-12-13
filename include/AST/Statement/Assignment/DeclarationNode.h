#ifndef CLIKE_DECLARATIONNODE_H
#define CLIKE_DECLARATIONNODE_H

#include "AST/ASTNode.h"

class DeclarationNode : public ASTNode {
public:
    DeclarationNode(Type type, ASTNode *identifier, int lineNumber) : ASTNode(lineNumber), type(type), identifier(identifier) {}
    ~DeclarationNode() override {
        delete identifier;
    }
    Type getType() { return type; }
    ASTNode *getIdentifier() { return identifier; }
private:
    Type type;
    ASTNode *identifier;
};

#endif //CLIKE_DECLARATIONNODE_H
