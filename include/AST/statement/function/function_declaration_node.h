#ifndef SYNTHSCRIPT_FUNCTIONDECLARATIONNODE_H
#define SYNTHSCRIPT_FUNCTIONDECLARATIONNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"
#include <utility>
#include <vector>

class FunctionDeclarationNode : public ASTNode {
public:
    FunctionDeclarationNode(std::vector<std::string> parameters, ASTNode *body, int line, int col)
        : ASTNode(line, col), parameters(std::move(parameters)), body(body) {}
    ~FunctionDeclarationNode() override { delete body; }

    NodeType get_node_type() const override { return FUNCTION_DECLARATION_NODE; }
    static NodeType get_node_type_static() { return FUNCTION_DECLARATION_NODE; }

    std::vector<std::string> *get_parameters() { return &parameters; }
    size_t get_parameters_size() { return parameters.size(); }
    std::string get_parameter(size_t index) { return parameters[index]; }
    ASTNode *get_body() { return body; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::vector<std::string> parameters;
    ASTNode *body;
};

#endif // SYNTHSCRIPT_FUNCTIONDECLARATIONNODE_H
