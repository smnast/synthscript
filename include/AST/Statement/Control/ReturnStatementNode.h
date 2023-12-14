#ifndef CLIKE_RETURNSTATEMENTNODE_H
#define CLIKE_RETURNSTATEMENTNODE_H

#include "AST/ASTNode.h"

class ReturnStatementNode : public ASTNode {
public:
    explicit ReturnStatementNode(ASTNode *value, int line, int col) : ASTNode(line, col), value(value) {}
    ~ReturnStatementNode() override {
        delete value;
    }
    ASTNode *getValue() { return value; }
    DECLARE_VISITOR_FUNCTIONS
private:
    ASTNode *value;
};

#endif //CLIKE_RETURNSTATEMENTNODE_H
