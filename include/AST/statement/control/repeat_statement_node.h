#ifndef SYNTHSCRIPT_REPEATSTATEMENTNODE_H
#define SYNTHSCRIPT_REPEATSTATEMENTNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class RepeatStatementNode : public ASTNode {
public:
    RepeatStatementNode(ASTNode *count, ASTNode *body, int line, int col)
        : ASTNode(line, col), count(count), body(body) {}
    ~RepeatStatementNode() override {
        delete count;
        delete body;
    }

    NodeType get_node_type() const override { return REPEAT_STATEMENT_NODE; }
    static NodeType get_node_type_static() { return REPEAT_STATEMENT_NODE; }

    ASTNode *get_count() { return count; }
    ASTNode *get_body() { return body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *count;
    ASTNode *body;
};

#endif // SYNTHSCRIPT_REPEATSTATEMENTNODE_H
