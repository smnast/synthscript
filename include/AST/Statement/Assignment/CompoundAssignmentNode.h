#ifndef CLIKE_COMPOUNDASSIGNMENTNODE_H
#define CLIKE_COMPOUNDASSIGNMENTNODE_H

#include <functional>
#include "AST/ASTNode.h"

class CompoundAssignmentNode : public ASTNode {
public:
    CompoundAssignmentNode(std::string op, ASTNode *identifier, ASTNode *value, int lineNumber) : ASTNode(lineNumber), op(std::move(op)), identifier(identifier), value(value) {}
    ~CompoundAssignmentNode() override {
        delete identifier;
        delete value;
    }
    std::string getOp() const { return op; }
    ASTNode *getIdentifier() const { return identifier; }
    ASTNode *getValue() const { return value; }
private:
    std::string op;
    ASTNode *identifier;
    ASTNode *value;
};

#endif //CLIKE_COMPOUNDASSIGNMENTNODE_H
