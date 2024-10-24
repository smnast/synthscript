#ifndef SYNTHSCRIPT_INTERPRETERVISITOR_H
#define SYNTHSCRIPT_INTERPRETERVISITOR_H

#include "built_in_functions.h"
#include "error_manager.h"
#include "object/object.h"
#include "symbol/symbol_table.h"
#include "visitor.h"
#include <memory>
#include <stack>

class InterpreterVisitor : public Visitor<std::shared_ptr<Object>, SymbolTable *> {
public:
    /**
     * @brief Construct a new InterpreterVisitor object
     * @param error_manager The error manager to use for error handling
     *
     * @note
     * The visitor does not take ownership of the program node or error manager.
     */
    InterpreterVisitor(ProgramNode *program_node, ErrorManager *error_manager);
    ~InterpreterVisitor() = default;

    void interpret();

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
    std::shared_ptr<Object> visit(CallOpNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(CompoundStatementNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(IdentifierNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(LiteralNode *node, SymbolTable *table) override;
    std::shared_ptr<Object> visit(ErrorNode *node, SymbolTable *table) override;

private:
    /**
     * @brief The error manager to use for error handling.
     */
    ErrorManager *error_manager;

    /**
     * @brief The root node of the program to visit.
     */
    ProgramNode *program_node;

    /**
     * @brief Report a runtime error.
     *
     * @param message The error message.
     * @param line The line of the error.
     * @param column The column of the error.
     */
    void runtime_error(const std::string &message, int line, int column);

    /**
     * @brief Built-in functions manager.
     */
    BuiltInFunctions built_in_functions;

    /**
     * @brief Stack of return values from each function call.
     */
    std::stack<std::shared_ptr<Object>> return_values;

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
