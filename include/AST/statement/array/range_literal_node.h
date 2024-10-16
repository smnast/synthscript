#ifndef SYNTHSCRIPT_RANGELITERALNODE_H
#define SYNTHSCRIPT_RANGELITERALNODE_H

#include "AST/AST_node.h"

class RangeLiteralNode : public ASTNode {
public:
    explicit RangeLiteralNode(ASTNode *start, ASTNode *end, int line, int col) : ASTNode(line, col), start(start), end(end) {}

    ~RangeLiteralNode() override {
        delete start;
        delete end;
    }

    ASTNode *get_start() { return start; }
    ASTNode *get_end() { return end; }

    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *start;
    ASTNode *end;
};

#endif // SYNTHSCRIPT_RANGELITERALNODE_H
