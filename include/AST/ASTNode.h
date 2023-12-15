#ifndef CLIKE_ASTNODE_H
#define CLIKE_ASTNODE_H

#include <iostream>

#include "Error.h"
#include "VisitFunctionsMacro.h"
#include "Types/Types.h"
#include "Visitor/Visitor.h"
#include "Visitor/PrintVisitor.h"
#include "Visitor/SemanticAnalysisVisitor.h"
#include "Visitor/InterpreterVisitor.h"

class ASTNode {
public:
    explicit ASTNode(int line, int col) : line(line), col(col) {};
    virtual ~ASTNode() = default;
    int getLineNumber() const { return line; }
    int getColumnNumber() const { return col; }
    virtual void accept(PrintVisitor *visitor, int arg) = 0;
    virtual void analyze(SemanticAnalysisVisitor *visitor, class SymbolTable *table) = 0;
    virtual std::shared_ptr<Object> evaluate(InterpreterVisitor *visitor, class SymbolTable *table) = 0;
private:
    int line, col;
};


#endif //CLIKE_ASTNODE_H
