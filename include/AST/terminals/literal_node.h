#ifndef SYNTHSCRIPT_LITERALNODE_H
#define SYNTHSCRIPT_LITERALNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class LiteralNode : public ASTNode {
public:
    LiteralNode(Type type, std::string value, int line, int col)
        : ASTNode(line, col), type(type), value(std::move(value)) {}
    ~LiteralNode() override = default;

    NodeType get_node_type() const override { return LITERAL_NODE; }
    static NodeType get_node_type_static() { return LITERAL_NODE; }

    Type get_type() { return type; }
    std::string get_value() { return value; }

    DECLARE_VISITOR_FUNCTIONS

private:
    Type type;
    std::string value;
};

#endif // SYNTHSCRIPT_LITERALNODE_H
