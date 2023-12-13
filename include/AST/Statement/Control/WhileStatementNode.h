#ifndef CLIKE_WHILESTATEMENTNODE_H
#define CLIKE_WHILESTATEMENTNODE_H

#include "AST/ASTNode.h"

class WhileStatementNode : public ASTNode {
public:
    WhileStatementNode(ASTNode *condition, ASTNode *body, int lineNumber) : ASTNode(lineNumber), condition(condition), body(body) {}
    ~WhileStatementNode() override {
        delete condition;
        delete body;
    }
    ASTNode *getCondition() { return condition; }
    ASTNode *getBody() { return body; }
private:
    ASTNode *condition, *body;
};

#endif //CLIKE_WHILESTATEMENTNODE_H
