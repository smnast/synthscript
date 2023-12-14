#ifndef CLIKE_SUBSCRIPTOPNODE_H
#define CLIKE_SUBSCRIPTOPNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class SubscriptOpNode : public ASTNode {
public:
    SubscriptOpNode(ASTNode *identifier, ASTNode *index, int line, int col) : ASTNode(line, col), identifier(identifier), index(index) {}
    ~SubscriptOpNode() override {
        delete identifier;
    }
    ASTNode *getIdentifier() { return identifier; }
    ASTNode *getIndex() { return index; }
    DECLARE_VISITOR_FUNCTIONS
private:
    ASTNode *identifier;
    ASTNode *index;
};

#endif //CLIKE_SUBSCRIPTOPNODE_H
