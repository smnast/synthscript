#ifndef CLIKE_WHILESTATEMENTNODE_H
#define CLIKE_WHILESTATEMENTNODE_H

#include "AST/ASTNode.h"

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

#endif //CLIKE_WHILESTATEMENTNODE_H
