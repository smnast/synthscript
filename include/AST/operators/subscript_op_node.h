#ifndef SYNTHSCRIPT_SUBSCRIPTOPNODE_H
#define SYNTHSCRIPT_SUBSCRIPTOPNODE_H

#include "AST/AST_node.h"
#include <utility>
#include <vector>

class SubscriptOpNode : public ASTNode {
public:
    SubscriptOpNode(ASTNode *identifier, ASTNode *index, int line, int col)
        : ASTNode(line, col), identifier(identifier), index(index) {}

    ~SubscriptOpNode() override {
        delete identifier;
        delete index;
    }

    ASTNode *get_identifier() { return identifier; }
    ASTNode *get_index() { return index; }
    DECLARE_VISITOR_FUNCTIONS

private:
    ASTNode *identifier;
    ASTNode *index;
};

#endif // SYNTHSCRIPT_SUBSCRIPTOPNODE_H
