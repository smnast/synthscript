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
    ASTNode *getStart() { return start; }
    ASTNode *getEnd() { return end; }
    DECLARE_VISITOR_FUNCTIONS
private:
    ASTNode *start, *end;
};

#endif //SYNTHSCRIPT_RANGELITERALNODE_H
