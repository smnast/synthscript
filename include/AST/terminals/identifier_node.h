#ifndef SYNTHSCRIPT_IDENTIFIERNODE_H
#define SYNTHSCRIPT_IDENTIFIERNODE_H

#include "AST/AST_node.h"
#include <utility>

class IdentifierNode : public ASTNode {
public:
    explicit IdentifierNode(std::string name, int line, int col)
        : ASTNode(line, col), name(std::move(name)) {}

    ~IdentifierNode() override = default;

    std::string get_name() const { return name; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::string name;
};

#endif //SYNTHSCRIPT_IDENTIFIERNODE_H
