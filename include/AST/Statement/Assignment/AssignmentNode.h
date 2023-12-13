#ifndef CLIKE_ASSIGNMENTNODE_H
#define CLIKE_ASSIGNMENTNODE_H

#include "AST/ASTNode.h"

class AssignmentNode : public ASTNode {
public:
    AssignmentNode(ASTNode *identifier, ASTNode *value, int line, int col) : ASTNode(line, col), identifier(identifier), value(value) {}
    ~AssignmentNode() override {
        delete identifier;
        delete value;
    }
    ASTNode *getIdentifier() { return identifier; }
    ASTNode *getValue() { return value; }
private:
    ASTNode *identifier;
    ASTNode *value;
};

#endif //CLIKE_ASSIGNMENTNODE_H
