#ifndef SYNTHSCRIPT_CALLNODE_H
#define SYNTHSCRIPT_CALLNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"
#include <utility>
#include <vector>

class CallNode : public ASTNode {
public:
    CallNode(std::string identifier, std::vector<ASTNode *> arguments, int line, int col)
        : ASTNode(line, col), arguments(std::move(arguments)), identifier(std::move(identifier)) {}
    ~CallNode() override {
        for (auto &argument : arguments) {
            delete argument;
        }
    }

    std::string get_identifier() const { return identifier; }
    std::vector<ASTNode *> *get_arguments() { return &arguments; }
    DECLARE_VISITOR_FUNCTIONS

private:
    std::string identifier;
    std::vector<ASTNode *> arguments;
};

#endif // SYNTHSCRIPT_FUNCTIONSTATEMENTNODE_H
