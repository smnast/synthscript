#ifndef SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H
#define SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H

#include "visitor.h"
#include "symbol/symbol_table.h"

class SemanticAnalysisVisitor : public Visitor<void, SymbolTable*> {
public:
    void visit(ProgramNode *node, SymbolTable* arg) override;
    void visit(BinOpNode *node, SymbolTable* arg) override;
    void visit(CastOpNode *node, SymbolTable* arg) override;
    void visit(SubscriptOpNode *node, SymbolTable* arg) override;
    void visit(UnaryOpNode *node, SymbolTable* arg) override;
    void visit(ArrayLiteralNode *node, SymbolTable* arg) override;
    void visit(RangeLiteralNode *node, SymbolTable* arg) override;
    void visit(AssignmentNode *node, SymbolTable* arg) override;
    void visit(BreakStatementNode *node, SymbolTable* arg) override;
    void visit(ContinueStatementNode *node, SymbolTable* arg) override;
    void visit(ReturnStatementNode *node, SymbolTable* arg) override;
    void visit(ForStatementNode *node, SymbolTable* arg) override;
    void visit(IfStatementNode *node, SymbolTable* arg) override;
    void visit(RepeatStatementNode *node, SymbolTable* arg) override;
    void visit(WhileStatementNode *node, SymbolTable* arg) override;
    void visit(FunctionDeclarationNode *node, SymbolTable* arg) override;
    void visit(FunctionStatementNode *node, SymbolTable* arg) override;
    void visit(CompoundStatementNode *node, SymbolTable* arg) override;
    void visit(IdentifierNode *node, SymbolTable* arg) override;
    void visit(LiteralNode *node, SymbolTable* arg) override;

    static std::string get_array_identifier(SubscriptOpNode *node, SymbolTable* arg);
    static void semantic_error(const std::string &message, int line, int column);
};

#endif //SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H
