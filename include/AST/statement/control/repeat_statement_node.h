#ifndef SYNTHSCRIPT_REPEATSTATEMENTNODE_H
#define SYNTHSCRIPT_REPEATSTATEMENTNODE_H

#include "AST/AST_node.h"
#include <utility>

class RepeatStatementNode : public ASTNode {
public:
    RepeatStatementNode(ASTNode *count, ASTNode *body, int line, int col) : ASTNode(line, col), count(count), body(body) {}

    ~RepeatStatementNode() override {
        delete count;
        delete body;
    }

    ASTNode *get_count() { return count; }
    ASTNode *get_body() { return body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *count;
    ASTNode *body;
};

#endif // SYNTHSCRIPT_REPEATSTATEMENTNODE_H
