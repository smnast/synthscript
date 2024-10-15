#include "visitor/print_visitor.h"
#include "AST/AST_nodes.h"

void PrintVisitor::visit(ProgramNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ProgramNode" << std::endl;
    for (auto &statement : *node->get_statements()) {
        statement->accept(this, arg+1);
    }
}

void PrintVisitor::visit(BinOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "BinOpNode " << tokenNames[node->get_op()] << std::endl;
    node->get_left_node()->accept(this, arg+1);
    node->get_right_node()->accept(this, arg+1);
}

void PrintVisitor::visit(CastOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "CastOpNode" << "'" << typeStrings[node->get_type()] << "'" << std::endl;
    node->get_operand()->accept(this, arg+1);
}

void PrintVisitor::visit(SubscriptOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "SubscriptOpNode" << std::endl;
    node->get_identifier()->accept(this, arg+1);
    node->get_index()->accept(this, arg+1);
}

void PrintVisitor::visit(UnaryOpNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "UnaryOpNode " << tokenNames[node->get_op()] << std::endl;
    node->get_operand()->accept(this, arg+1);
}

void PrintVisitor::visit(ArrayLiteralNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ArrayLiteralNode" << std::endl;
    for (auto &element : *node->get_values()) {
        element->accept(this, arg+1);
    }
}

void PrintVisitor::visit(RangeLiteralNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "RangeLiteralNode" << std::endl;
    node->get_start()->accept(this, arg+1);
    node->get_end()->accept(this, arg+1);
}

void PrintVisitor::visit(AssignmentNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "AssignmentNode" << std::endl;
    node->get_identifier()->accept(this, arg+1);
    node->get_value()->accept(this, arg+1);
}

void PrintVisitor::visit(BreakStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "BreakStatementNode" << std::endl;
}

void PrintVisitor::visit(ContinueStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ContinueStatementNode" << std::endl;
}

void PrintVisitor::visit(ReturnStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ReturnStatementNode" << std::endl;
    node->get_value()->accept(this, arg+1);
}

void PrintVisitor::visit(ForStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "ForStatementNode" << std::endl;
    std::cout << std::string(arg+1, '\t') << node->get_identifier() << std::endl;
    node->get_iterable()->accept(this, arg+1);
    node->get_body()->accept(this, arg+1);
}

void PrintVisitor::visit(IfStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "IfStatementNode" << std::endl;
    node->get_condition()->accept(this, arg+1);
    node->get_if_body()->accept(this, arg+1);
    if (node->get_else_body() != nullptr) {
        node->get_else_body()->accept(this, arg+1);
    }
}

void PrintVisitor::visit(RepeatStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "RepeatStatementNode" << std::endl;
    node->get_count()->accept(this, arg+1);
    node->get_body()->accept(this, arg+1);
}

void PrintVisitor::visit(WhileStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "WhileStatementNode" << std::endl;
    node->get_condition()->accept(this, arg+1);
    node->get_body()->accept(this, arg+1);
}

void PrintVisitor::visit(FunctionDeclarationNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "FunctionDeclarationNode" << std::endl;
    std::cout << std::string(arg+1, '\t') << node->get_identifier() << std::endl;
    for (auto &param : *node->get_parameters()) {
        std::cout << std::string(arg+1, '\t') << param << std::endl;
    }
}

void PrintVisitor::visit(FunctionStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "FunctionStatementNode" << std::endl;
    std::cout << std::string(arg+1, '\t') << node->get_identifier() << std::endl;
    for (auto &param : *node->get_arguments()) {
        param->accept(this, arg+1);
    }
}

void PrintVisitor::visit(CompoundStatementNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "CompoundStatementNode" << std::endl;
    for (auto &statement : *node->get_statements()) {
        statement->accept(this, arg+1);
    }
}

void PrintVisitor::visit(IdentifierNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "IdentifierNode " << "'" << node->get_name() << "'" << std::endl;
}

void PrintVisitor::visit(LiteralNode *node, int arg) {
    std::cout << std::string(arg, '\t') << "LiteralNode " << "'" << node->get_value() << "'" << std::endl;
}
