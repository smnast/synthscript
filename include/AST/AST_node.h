#ifndef SYNTHSCRIPT_ASTNODE_H
#define SYNTHSCRIPT_ASTNODE_H

#include "visitor/interpreter_visitor.h"
#include "visitor/print_visitor.h"
#include "visitor/semantic_analysis_visitor.h"

class ASTNode {
public:
    ASTNode(int line, int col) : line(line), col(col) {}
    virtual ~ASTNode() = default;

    int get_line() const { return line; }
    int get_column() const { return col; }

    virtual void accept(PrintVisitor *visitor, int arg) = 0;
    virtual void analyze(SemanticAnalysisVisitor *visitor, class SymbolTable *table) = 0;
    virtual std::shared_ptr<Object> evaluate(InterpreterVisitor *visitor,
                                             class SymbolTable *table) = 0;

private:
    int line;
    int col;
};

#endif // SYNTHSCRIPT_ASTNODE_H
