#ifndef SYNTHSCRIPT_RETURNSTATEMENTNODE_H
#define SYNTHSCRIPT_RETURNSTATEMENTNODE_H

#include "AST/AST_node.h"

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

#endif //SYNTHSCRIPT_RETURNSTATEMENTNODE_H
