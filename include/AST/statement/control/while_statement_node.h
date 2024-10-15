#ifndef SYNTHSCRIPT_WHILESTATEMENTNODE_H
#define SYNTHSCRIPT_WHILESTATEMENTNODE_H

#include "AST/AST_node.h"

class WhileStatementNode : public ASTNode {
public:
    WhileStatementNode(ASTNode *condition, ASTNode *body, int line, int col) : ASTNode(line, col), condition(condition), body(body) {}
    ~WhileStatementNode() override {
        delete condition;
        delete body;
    }
    ASTNode *getCondition() { return condition; }
    ASTNode *getBody() { return body; }
    DECLARE_VISITOR_FUNCTIONS
private:
    ASTNode *condition, *body;
};

#endif //SYNTHSCRIPT_WHILESTATEMENTNODE_H
