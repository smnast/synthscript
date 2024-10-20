#ifndef SYNTHSCRIPT_PRINTVISITOR_H
#define SYNTHSCRIPT_PRINTVISITOR_H

#include "visitor.h"

class PrintVisitor : public Visitor<void, int> {
public:
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
    void visit(CallNode *node, int indentation) override;
    void visit(CompoundStatementNode *node, int indentation) override;
    void visit(IdentifierNode *node, int indentation) override;
    void visit(LiteralNode *node, int indentation) override;
    void visit(ErrorNode *node, int indentation) override;
};

#endif // SYNTHSCRIPT_PRINTVISITOR_H
