#ifndef CLIKE_FUNCTIONSTATEMENTNODE_H
#define CLIKE_FUNCTIONSTATEMENTNODE_H

#include <utility>
#include <vector>

#include "AST/ASTNode.h"

class FunctionStatementNode : public ASTNode {
public:
    FunctionStatementNode(std::string identifier, std::vector<ASTNode*> arguments, int line, int col) : ASTNode(line, col), arguments(std::move(arguments)), identifier(std::move(identifier)) {}
    ~FunctionStatementNode() override = default;
    std::string getIdentifier() const { return identifier; }
    std::vector<ASTNode*> *getArguments() { return &arguments; }
    DECLARE_VISITOR_FUNCTIONS
private:
    std::string identifier;
    std::vector<ASTNode*> arguments;
};

#endif //CLIKE_FUNCTIONSTATEMENTNODE_H
