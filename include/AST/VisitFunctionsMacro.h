#ifndef CLIKE_VISITFUNCTIONSMACRO_H
#define CLIKE_VISITFUNCTIONSMACRO_H

#define DECLARE_VISITOR_FUNCTIONS \
void accept(PrintVisitor *visitor, int indent) override { return visitor->visit(this, indent); } \
Type analyze(SemanticAnalysisVisitor *visitor, SymbolTable *table) override { return visitor->visit(this, table); } \
std::shared_ptr<Object> evaluate(InterpreterVisitor *visitor, SymbolTable *table) override { return visitor->visit(this, table); }

#endif //CLIKE_VISITFUNCTIONSMACRO_H
