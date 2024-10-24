#ifndef SYNTHSCRIPT_CALLOPNODE_H
#define SYNTHSCRIPT_CALLOPNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"
#include <utility>
#include <vector>

class CallOpNode : public ASTNode {
public:
    CallOpNode(std::string identifier, std::vector<ASTNode *> arguments, int line, int col)
        : ASTNode(line, col), arguments(std::move(arguments)), identifier(std::move(identifier)) {}
    ~CallOpNode() override {
        for (auto &argument : arguments) {
            delete argument;
        }
    }

    NodeType get_node_type() const override { return CALL_NODE; }
    static NodeType get_node_type_static() { return CALL_NODE; }

    std::string get_identifier() const { return identifier; }

    std::vector<ASTNode *> *get_arguments() { return &arguments; }
    size_t get_arguments_size() { return arguments.size(); }
    ASTNode *get_argument(size_t index) { return arguments[index]; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::string identifier;
    std::vector<ASTNode *> arguments;
};

#endif // SYNTHSCRIPT_CALLOPNODE_H
