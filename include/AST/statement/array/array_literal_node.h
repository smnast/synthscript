#ifndef SYNTHSCRIPT_ARRAYLITERALNODE_H
#define SYNTHSCRIPT_ARRAYLITERALNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class ArrayLiteralNode : public ASTNode {
public:
    ArrayLiteralNode(std::vector<ASTNode *> values, int line, int col)
        : ASTNode(line, col), values(std::move(values)) {}
    ~ArrayLiteralNode() override {
        for (auto &value : values) {
            delete value;
        }
    }

    NodeType get_node_type() const override { return ARRAY_LITERAL_NODE; }
    static NodeType get_node_type_static() { return ARRAY_LITERAL_NODE; }

    std::vector<ASTNode *> *get_values() { return &values; }
    size_t get_values_size() const { return values.size(); }
    ASTNode *get_value(size_t index) const { return values[index]; }

    DECLARE_VISITOR_FUNCTIONS

private:
    std::vector<ASTNode *> values;
};

#endif // SYNTHSCRIPT_ARRAYLITERALNODE_H
