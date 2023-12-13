#ifndef CLIKE_INITIALIZATIONNODE_H
#define CLIKE_INITIALIZATIONNODE_H

#include "AST/ASTNode.h"

class InitializationNode : public ASTNode {
public:
    InitializationNode(Type type, ASTNode *identifier, ASTNode *value, int lineNumber) : ASTNode(lineNumber), type(type), identifier(identifier), value(value) {}
    ~InitializationNode() override {
        delete identifier;
        delete value;
    }
    Type getType() { return type; }
    ASTNode *getIdentifier() { return identifier; }
    ASTNode *getValue() { return value; }
private:
    Type type;
    ASTNode *identifier;
    ASTNode *value;
};

#endif //CLIKE_INITIALIZATIONNODE_H
