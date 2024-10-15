#ifndef SYNTHSCRIPT_INTERPRETERVISITOR_H
#define SYNTHSCRIPT_INTERPRETERVISITOR_H

#include "visitor.h"
#include "object/object.h"
#include "symbol/symbol_table.h"
#include <memory>

class InterpreterVisitor : public Visitor<std::shared_ptr<Object>, SymbolTable*> {
public:
    std::shared_ptr<Object> visit(ProgramNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(BinOpNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(CastOpNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(SubscriptOpNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(UnaryOpNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(ArrayLiteralNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(RangeLiteralNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(AssignmentNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(BreakStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(ContinueStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(ReturnStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(ForStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(IfStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(RepeatStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(WhileStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(FunctionDeclarationNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(FunctionStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(CompoundStatementNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(IdentifierNode *node, SymbolTable* arg) override;
    std::shared_ptr<Object> visit(LiteralNode *node, SymbolTable* arg) override;

private:
    std::shared_ptr<Object> returnVal = nullptr;
    bool backtracking = false, breaking = false, returning = false;
};

#endif //SYNTHSCRIPT_INTERPRETERVISITOR_H
