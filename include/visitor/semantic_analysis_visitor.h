#ifndef SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H
#define SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H

#include "symbol/symbol_table.h"
#include "visitor.h"

class SemanticAnalysisVisitor : public Visitor<void, SymbolTable *> {
public:
    void visit(ProgramNode *node, SymbolTable *table) override;
    void visit(BinOpNode *node, SymbolTable *table) override;
    void visit(CastOpNode *node, SymbolTable *table) override;
    void visit(SubscriptOpNode *node, SymbolTable *table) override;
    void visit(UnaryOpNode *node, SymbolTable *table) override;
    void visit(ArrayLiteralNode *node, SymbolTable *table) override;
    void visit(RangeLiteralNode *node, SymbolTable *table) override;
    void visit(AssignmentNode *node, SymbolTable *table) override;
    void visit(BreakStatementNode *node, SymbolTable *table) override;
    void visit(ContinueStatementNode *node, SymbolTable *table) override;
    void visit(ReturnStatementNode *node, SymbolTable *table) override;
    void visit(ForStatementNode *node, SymbolTable *table) override;
    void visit(IfStatementNode *node, SymbolTable *table) override;
    void visit(RepeatStatementNode *node, SymbolTable *table) override;
    void visit(WhileStatementNode *node, SymbolTable *table) override;
    void visit(FunctionDeclarationNode *node, SymbolTable *table) override;
    void visit(CallNode *node, SymbolTable *table) override;
    void visit(CompoundStatementNode *node, SymbolTable *table) override;
    void visit(IdentifierNode *node, SymbolTable *table) override;
    void visit(LiteralNode *node, SymbolTable *table) override;
    void visit(ErrorNode *node, SymbolTable *table) override;

    static std::string get_array_identifier(SubscriptOpNode *node, SymbolTable *table);
    static void semantic_error(const std::string &message, int line, int column);
};

#endif // SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H
