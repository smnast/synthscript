#ifndef CLIKE_SUBSCRIPTOPNODE_H
#define CLIKE_SUBSCRIPTOPNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class SubscriptOpNode : public ASTNode {
public:
    SubscriptOpNode(ASTNode *identifier, std::vector<ASTNode*> indices, int lineNumber) : ASTNode(lineNumber), identifier(identifier), indices(std::move(indices)) {}
    ~SubscriptOpNode() override {
        delete identifier;
        for (auto &index : indices) {
            delete index;
        }
    }
    ASTNode *getIdentifier() { return identifier; }
    std::vector<ASTNode*> *getIndices() { return &indices; }
private:
    ASTNode *identifier;
    std::vector<ASTNode*> indices;
};

#endif //CLIKE_SUBSCRIPTOPNODE_H
