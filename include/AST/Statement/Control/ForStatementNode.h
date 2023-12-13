#ifndef CLIKE_FORSTATEMENTNODE_H
#define CLIKE_FORSTATEMENTNODE_H

#include "AST/ASTNode.h"

class ForStatementNode : public ASTNode {
public:
    ForStatementNode(ASTNode *initStatement, ASTNode *condition, ASTNode *updateStatement, ASTNode *body, int lineNumber) : ASTNode(lineNumber), initStatement(initStatement), condition(condition), updateStatement(updateStatement), body(body) {}
    ~ForStatementNode() override {
        delete initStatement;
        delete condition;
        delete updateStatement;
        delete body;
    }
    ASTNode *getInitStatement() { return initStatement; }
    ASTNode *getCondition() { return condition; }
    ASTNode *getUpdateStatement() { return updateStatement; }
    ASTNode *getBody() { return body; }
private:
    ASTNode *initStatement, *condition, *updateStatement, *body;
};

#endif //CLIKE_FORSTATEMENTNODE_H
