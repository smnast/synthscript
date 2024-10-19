#ifndef SYNTHSCRIPT_ERRORNODE_H
#define SYNTHSCRIPT_ERRORNODE_H

#include "AST/AST_node.h"
#include "AST/visit_functions_macro.h"

class ErrorNode : public ASTNode {
public:
    ErrorNode(int line, int col) : ASTNode(line, col) {}
    ~ErrorNode() override = default;

    DECLARE_VISITOR_FUNCTIONS
};

#endif // SYNTHSCRIPT_IDENTIFIERNODE_H