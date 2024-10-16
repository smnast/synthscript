#ifndef SYNTHSCRIPT_ASSIGNMENTNODE_H
#define SYNTHSCRIPT_ASSIGNMENTNODE_H

#include "AST/AST_node.h"

class AssignmentNode : public ASTNode {
public:
    AssignmentNode(ASTNode *identifier, ASTNode *value, int line, int col)
        : ASTNode(line, col), identifier(identifier), value(value) {}

    ~AssignmentNode() override {
        delete identifier;
        delete value;
    }

    ASTNode *get_identifier() { return identifier; }
    ASTNode *get_value() { return value; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *identifier;
    ASTNode *value;
};

#endif // SYNTHSCRIPT_ASSIGNMENTNODE_H
