#ifndef SYNTHSCRIPT_VISITOR_H
#define SYNTHSCRIPT_VISITOR_H

#include "AST/node_forward_classes.h"
#include "built_in_functions.h"

template <typename T, typename A> class Visitor {
public:
    virtual T visit(ProgramNode *node, A arg) = 0;
    virtual T visit(BinOpNode *node, A arg) = 0;
    virtual T visit(CastOpNode *node, A arg) = 0;
    virtual T visit(SubscriptOpNode *node, A arg) = 0;
    virtual T visit(UnaryOpNode *node, A arg) = 0;
    virtual T visit(ArrayLiteralNode *node, A arg) = 0;
    virtual T visit(RangeLiteralNode *node, A arg) = 0;
    virtual T visit(AssignmentNode *node, A arg) = 0;
    virtual T visit(BreakStatementNode *node, A arg) = 0;
    virtual T visit(ContinueStatementNode *node, A arg) = 0;
    virtual T visit(ReturnStatementNode *node, A arg) = 0;
    virtual T visit(ForStatementNode *node, A arg) = 0;
    virtual T visit(IfStatementNode *node, A arg) = 0;
    virtual T visit(RepeatStatementNode *node, A arg) = 0;
    virtual T visit(WhileStatementNode *node, A arg) = 0;
    virtual T visit(FunctionDeclarationNode *node, A arg) = 0;
    virtual T visit(FunctionStatementNode *node, A arg) = 0;
    virtual T visit(CompoundStatementNode *node, A arg) = 0;
    virtual T visit(IdentifierNode *node, A arg) = 0;
    virtual T visit(LiteralNode *node, A arg) = 0;
};

#endif // SYNTHSCRIPT_VISITOR_H
