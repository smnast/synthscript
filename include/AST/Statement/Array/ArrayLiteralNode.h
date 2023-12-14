#ifndef SYNTHSCRIPT_ARRAYLITERALNODE_H
#define SYNTHSCRIPT_ARRAYLITERALNODE_H

#include "AST/ASTNode.h"

class ArrayLiteralNode : public ASTNode {
public:
    explicit ArrayLiteralNode(std::vector<ASTNode*> values, int line, int col) : ASTNode(line, col), values(std::move(values)) {}
    ~ArrayLiteralNode() override = default;
    std::vector<ASTNode*> *getValues() { return &values; }
    DECLARE_VISITOR_FUNCTIONS
private:
    std::vector<ASTNode*> values;
};

#endif //SYNTHSCRIPT_ARRAYLITERALNODE_H
