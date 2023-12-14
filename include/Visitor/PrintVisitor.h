#ifndef SYNTHSCRIPT_PRINTVISITOR_H
#define SYNTHSCRIPT_PRINTVISITOR_H

#include "Visitor.h"

class PrintVisitor : public Visitor<void, int> {
public:
    void visit(ProgramNode *node, int arg) override;
    void visit(BinOpNode *node, int arg) override;
    void visit(CastOpNode *node, int arg) override;
    void visit(SubscriptOpNode *node, int arg) override;
    void visit(UnaryOpNode *node, int arg) override;
    void visit(ArrayLiteralNode *node, int arg) override;
    void visit(AssignmentNode *node, int arg) override;
    void visit(BreakStatementNode *node, int arg) override;
    void visit(ContinueStatementNode *node, int arg) override;
    void visit(ReturnStatementNode *node, int arg) override;
    void visit(ForStatementNode *node, int arg) override;
    void visit(IfStatementNode *node, int arg) override;
    void visit(WhileStatementNode *node, int arg) override;
    void visit(FunctionDeclarationNode *node, int arg) override;
    void visit(FunctionStatementNode *node, int arg) override;
    void visit(CompoundStatementNode *node, int arg) override;
    void visit(IdentifierNode *node, int arg) override;
    void visit(LiteralNode *node, int arg) override;
};

#endif //SYNTHSCRIPT_PRINTVISITOR_H
