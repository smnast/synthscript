#ifndef SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H
#define SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H

#include "built_in_functions.h"
#include "error_manager.h"
#include "symbol/symbol_table.h"
#include "visitor.h"

class SemanticAnalysisVisitor : public Visitor<void, SymbolTable *> {
public:
    /**
     * @brief Construct a new InterpreterVisitor object
     * @param error_manager The error manager to use for error handling
     *
     * @note
     * The visitor does not take ownership of the program node or error manager.
     */
    SemanticAnalysisVisitor(ProgramNode *program_node, ErrorManager *error_manager);
    ~SemanticAnalysisVisitor() = default;

    void analyze();

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
    void visit(CallOpNode *node, SymbolTable *table) override;
    void visit(CompoundStatementNode *node, SymbolTable *table) override;
    void visit(IdentifierNode *node, SymbolTable *table) override;
    void visit(LiteralNode *node, SymbolTable *table) override;
    void visit(ErrorNode *node, SymbolTable *table) override;

    /**
     * @brief Get the array identifier object from a (possibly nested) subscript operation node.
     *
     * @param node The node to get the identifier from.
     * @param table The symbol table.
     * @return The identifier of the array.
     */
    std::string get_array_identifier(SubscriptOpNode *node, SymbolTable *table);

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
     * @brief Built-in functions manager.
     */
    BuiltInFunctions built_in_functions;

    /**
     * @brief Report a semantic error.
     *
     * @param message The error message.
     * @param line The line of the error.
     * @param column The column of the error.
     */
    void semantic_error(const std::string &message, int line, int column);
};

#endif // SYNTHSCRIPT_SEMANTICANALYSISVISITOR_H
