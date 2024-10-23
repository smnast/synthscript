#ifndef SYNTHSCRIPT_ASTNODE_H
#define SYNTHSCRIPT_ASTNODE_H

#include "visitor/interpreter_visitor.h"
#include "visitor/print_visitor.h"
#include "visitor/semantic_analysis_visitor.h"

typedef enum {
    BIN_OP_NODE,
    CALL_NODE,
    CAST_OP_NODE,
    SUBSCRIPT_OP_NODE,
    UNARY_OP_NODE,
    ARRAY_LITERAL_NODE,
    RANGE_LITERAL_NODE,
    ASSIGNMENT_NODE,
    BREAK_STATEMENT_NODE,
    CONTINUE_STATEMENT_NODE,
    FOR_STATEMENT_NODE,
    IF_STATEMENT_NODE,
    REPEAT_STATEMENT_NODE,
    RETURN_STATEMENT_NODE,
    WHILE_STATEMENT_NODE,
    FUNCTION_DECLARATION_NODE,
    COMPOUND_STATEMENT_NODE,
    IDENTIFIER_NODE,
    LITERAL_NODE,
    ERROR_NODE,
    PROGRAM_NODE
} NodeType;

class ASTNode {
public:
    ASTNode(int line, int col) : line(line), col(col) {}
    virtual ~ASTNode() = default;

    int get_line() const { return line; }
    int get_column() const { return col; }

    virtual NodeType get_node_type() const = 0;

    virtual void accept(PrintVisitor *visitor, int arg) = 0;
    virtual void analyze(SemanticAnalysisVisitor *visitor, class SymbolTable *table) = 0;
    virtual std::shared_ptr<Object> evaluate(InterpreterVisitor *visitor,
                                             class SymbolTable *table) = 0;

private:
    int line;
    int col;
};

#endif // SYNTHSCRIPT_ASTNODE_H
