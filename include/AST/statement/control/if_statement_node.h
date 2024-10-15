#ifndef SYNTHSCRIPT_IFSTATEMENTNODE_H
#define SYNTHSCRIPT_IFSTATEMENTNODE_H

#include "AST/AST_node.h"

class IfStatementNode : public ASTNode {
public:
    IfStatementNode(ASTNode *condition, ASTNode *ifBody, ASTNode *elseBody, int line, int col) : ASTNode(line, col), condition(condition), ifBody(ifBody), elseBody(elseBody) {}
    ~IfStatementNode() override {
        delete condition;
        delete ifBody;
        delete elseBody;
    }
    ASTNode *getCondition() { return condition; }
    ASTNode *getIfBody() { return ifBody; }
    ASTNode *getElseBody() { return elseBody; }
    DECLARE_VISITOR_FUNCTIONS
private:
    ASTNode *condition, *ifBody, *elseBody;
};

#endif //SYNTHSCRIPT_IFSTATEMENTNODE_H
