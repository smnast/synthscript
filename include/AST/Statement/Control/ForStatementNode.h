#ifndef CLIKE_FORSTATEMENTNODE_H
#define CLIKE_FORSTATEMENTNODE_H

#include "AST/ASTNode.h"

class ForStatementNode : public ASTNode {
public:
    ForStatementNode(ASTNode *initStatement, ASTNode *start, ASTNode *end, int line, int col) : ASTNode(line, col), initStatement(initStatement), start(start), end(end) {}
    ~ForStatementNode() override {
        delete initStatement;
        delete start;
        delete end;
    }
    ASTNode *getInitStatement() { return initStatement; }
    ASTNode *getStart() { return start; }
    ASTNode *getEnd() { return end; }
private:
    ASTNode *initStatement, *start, *end;
};

#endif //CLIKE_FORSTATEMENTNODE_H
