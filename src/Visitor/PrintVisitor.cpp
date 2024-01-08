#include "Visitor/PrintVisitor.h"
#include "AST/ASTNodes.h"

void PrintVisitor::visit(ProgramNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ProgramNode" << std::endl;
    for (auto &statement : *node->getStatements()) {
        statement->accept(this, arg+1);
    }
}

void PrintVisitor::visit(BinOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "BinOpNode " << tokenNames[node->getOp()] << std::endl;
    node->getLeftNode()->accept(this, arg+1);
    node->getRightNode()->accept(this, arg+1);
}

void PrintVisitor::visit(CastOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "CastOpNode" << "'" << typeStrings[node->getType()] << "'" << std::endl;
    node->getOperand()->accept(this, arg+1);
}

void PrintVisitor::visit(SubscriptOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "SubscriptOpNode" << std::endl;
    node->getIdentifier()->accept(this, arg+1);
    node->getIndex()->accept(this, arg+1);
}

void PrintVisitor::visit(UnaryOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "UnaryOpNode " << tokenNames[node->getOp()] << std::endl;
    node->getOperand()->accept(this, arg+1);
}

void PrintVisitor::visit(ArrayLiteralNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ArrayLiteralNode" << std::endl;
    for (auto &element : *node->getValues()) {
        element->accept(this, arg+1);
    }
}

void PrintVisitor::visit(RangeLiteralNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "RangeLiteralNode" << std::endl;
    node->getStart()->accept(this, arg+1);
    node->getEnd()->accept(this, arg+1);
}

void PrintVisitor::visit(AssignmentNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "AssignmentNode" << std::endl;
    node->getIdentifier()->accept(this, arg+1);
    node->getValue()->accept(this, arg+1);
}

void PrintVisitor::visit(BreakStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "BreakStatementNode" << std::endl;
}

void PrintVisitor::visit(ContinueStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ContinueStatementNode" << std::endl;
}

void PrintVisitor::visit(ReturnStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ReturnStatementNode" << std::endl;
    node->getValue()->accept(this, arg+1);
}

void PrintVisitor::visit(ForStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ForStatementNode" << std::endl;
    std::cout << std::string(arg+1, '\t') << node->getIdentifier() << std::endl;
    node->getIterable()->accept(this, arg+1);
    node->getBody()->accept(this, arg+1);
}

void PrintVisitor::visit(IfStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "IfStatementNode" << std::endl;
    node->getCondition()->accept(this, arg+1);
    node->getIfBody()->accept(this, arg+1);
    if (node->getElseBody() != nullptr) {
        node->getElseBody()->accept(this, arg+1);
    }
}

void PrintVisitor::visit(RepeatStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "RepeatStatementNode" << std::endl;
    node->getCount()->accept(this, arg+1);
    node->getBody()->accept(this, arg+1);
}

void PrintVisitor::visit(WhileStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "WhileStatementNode" << std::endl;
    node->getCondition()->accept(this, arg+1);
    node->getBody()->accept(this, arg+1);
}

void PrintVisitor::visit(FunctionDeclarationNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "FunctionDeclarationNode" << std::endl;
    std::cout << std::string(arg+1, '\t') << node->getIdentifier() << std::endl;
    for (auto &param : *node->getParameters()) {
        std::cout << std::string(arg+1, '\t') << param << std::endl;
    }
}

void PrintVisitor::visit(FunctionStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "FunctionStatementNode" << std::endl;
    std::cout << std::string(arg+1, '\t') << node->getIdentifier() << std::endl;
    for (auto &param : *node->getArguments()) {
        param->accept(this, arg+1);
    }
}

void PrintVisitor::visit(CompoundStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "CompoundStatementNode" << std::endl;
    for (auto &statement : *node->getStatements()) {
        statement->accept(this, arg+1);
    }
}

void PrintVisitor::visit(IdentifierNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "IdentifierNode " << "'" << node->getName() << "'" << std::endl;
}

void PrintVisitor::visit(LiteralNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "LiteralNode " << "'" << node->getValue() << "'" << std::endl;
}
