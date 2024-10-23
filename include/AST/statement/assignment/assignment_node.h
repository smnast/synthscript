#ifndef SYNTHSCRIPT_ASSIGNMENTNODE_H
#define SYNTHSCRIPT_ASSIGNMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class AssignmentNode : public ASTNode {
public:
    AssignmentNode(ASTNode *identifier, ASTNode *value, int line, int col)
        : ASTNode(line, col), identifier(identifier), value(value) {}
    ~AssignmentNode() override {
        delete identifier;
        delete value;
    }

    NodeType get_node_type() const override { return ASSIGNMENT_NODE; }

    ASTNode *get_identifier() { return identifier; }
    ASTNode *get_value() { return value; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *identifier;
    ASTNode *value;
};

#endif // SYNTHSCRIPT_ASSIGNMENTNODE_H
