#ifndef SYNTHSCRIPT_PRINTVISITOR_H
#define SYNTHSCRIPT_PRINTVISITOR_H

#include "error_manager.h"
#include "built_in_functions.h"
#include "visitor.h"

class PrintVisitor : public Visitor<void, int> {
public:
    /**
     * @brief Construct a new InterpreterVisitor object
     * @param error_manager The error manager to use for error handling
     *
     * @note
     * The visitor does not take ownership of the program node or error manager.
     */
    PrintVisitor(ProgramNode *program_node, ErrorManager *error_manager);
    ~PrintVisitor() = default;

    void print();

    void visit(ProgramNode *node, int indentation) override;
    void visit(BinOpNode *node, int indentation) override;
    void visit(CastOpNode *node, int indentation) override;
    void visit(SubscriptOpNode *node, int indentation) override;
    void visit(UnaryOpNode *node, int indentation) override;
    void visit(ArrayLiteralNode *node, int indentation) override;
    void visit(RangeLiteralNode *node, int indentation) override;
    void visit(AssignmentNode *node, int indentation) override;
    void visit(BreakStatementNode *node, int indentation) override;
    void visit(ContinueStatementNode *node, int indentation) override;
    void visit(ReturnStatementNode *node, int indentation) override;
    void visit(ForStatementNode *node, int indentation) override;
    void visit(IfStatementNode *node, int indentation) override;
    void visit(RepeatStatementNode *node, int indentation) override;
    void visit(WhileStatementNode *node, int indentation) override;
    void visit(FunctionDeclarationNode *node, int indentation) override;
    void visit(CallOpNode *node, int indentation) override;
    void visit(CompoundStatementNode *node, int indentation) override;
    void visit(IdentifierNode *node, int indentation) override;
    void visit(LiteralNode *node, int indentation) override;
    void visit(ErrorNode *node, int indentation) override;

private:
    /**
     * @brief The error manager to use for error handling.
     */
    ErrorManager *error_manager;

    /**
     * @brief The root node of the program to visit.
     */
    ProgramNode *program_node;
};

#endif // SYNTHSCRIPT_PRINTVISITOR_H
