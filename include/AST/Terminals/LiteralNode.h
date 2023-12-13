#ifndef CLIKE_LITERALNODE_H
#define CLIKE_LITERALNODE_H

#include "AST/ASTNode.h"

class LiteralNode : public ASTNode {
public:
    explicit LiteralNode(Type type, std::string value, int lineNumber) : ASTNode(lineNumber), type(type), value(std::move(value)) {}
    ~LiteralNode() override = default;
    Type getType() { return type; }
    std::string getValue() { return value; }
private:
    Type type;
    std::string value;
};

#endif //CLIKE_LITERALNODE_H
