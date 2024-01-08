#ifndef SYNTHSCRIPT_REPEATSTATEMENTNODE_H
#define SYNTHSCRIPT_REPEATSTATEMENTNODE_H

#include <utility>

#include "AST/ASTNode.h"

class RepeatStatementNode : public ASTNode {
public:
    RepeatStatementNode(ASTNode *count, ASTNode *body, int line, int col) : ASTNode(line, col), count(count), body(body) {}
    ~RepeatStatementNode() override {
        delete count;
        delete body;
    }
    ASTNode *getCount() { return count; }
    ASTNode *getBody() { return body; }
    DECLARE_VISITOR_FUNCTIONS
private:
    ASTNode *count, *body;
};

#endif //SYNTHSCRIPT_REPEATSTATEMENTNODE_H
