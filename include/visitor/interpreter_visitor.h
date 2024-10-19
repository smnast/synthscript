#ifndef SYNTHSCRIPT_INTERPRETERVISITOR_H
#define SYNTHSCRIPT_INTERPRETERVISITOR_H

#include "object/object.h"
#include "symbol/symbol_table.h"
#include "visitor.h"
#include <memory>

class InterpreterVisitor : public Visitor<std::shared_ptr<Object>, SymbolTable *> {
public:
    std::shared_ptr<Object> visit(ProgramNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(BinOpNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(CastOpNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(SubscriptOpNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(UnaryOpNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(ArrayLiteralNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(RangeLiteralNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(AssignmentNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(BreakStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(ContinueStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(ReturnStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(ForStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(IfStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(RepeatStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(WhileStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(FunctionDeclarationNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(FunctionStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(CompoundStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(IdentifierNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(LiteralNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(ErrorNode *node, SymbolTable *table) override;

private:
    /**
     * @brief Current return value of the function.
     */
    std::shared_ptr<Object> return_val = nullptr;
    
    /**
     * @brief Stores if the interpreter is backtracking out of a node.
     * 
     * Backtracking will stop after reaching node like a loop.
     */
    bool backtracking = false;

    /**
     * @brief Stores if the interpreter is breaking out of a loop.
     */
    bool breaking = false;

    /**
     * @brief Stores if the interpreter is returning from a function.
     * 
     * Returning will not stop until reaching a function node.
     */
    bool returning = false;
};

#endif // SYNTHSCRIPT_INTERPRETERVISITOR_H
