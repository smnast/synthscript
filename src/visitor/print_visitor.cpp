#include "visitor/print_visitor.h"
#include "AST/AST_nodes.h"
#include <iostream>

PrintVisitor::PrintVisitor(ProgramNode *program_node, ErrorManager *error_manager)
    : program_node(program_node), error_manager(error_manager) {}

void PrintVisitor::print() {
    program_node->accept(this, 0);
}

void PrintVisitor::visit(ProgramNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "ProgramNode" << std::endl;

    for (auto &statement : *node->get_statements()) {
        statement->accept(this, indentation + 1);
    }
}

void PrintVisitor::visit(BinOpNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "BinOpNode " << token_values[node->get_op()]
              << std::endl;

    node->get_left_node()->accept(this, indentation + 1);
    node->get_right_node()->accept(this, indentation + 1);
}

void PrintVisitor::visit(CastOpNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "CastOpNode"
              << "'" << type_to_string(node->get_type()) << "'" << std::endl;

    node->get_operand()->accept(this, indentation + 1);
}

void PrintVisitor::visit(SubscriptOpNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "SubscriptOpNode" << std::endl;

    node->get_identifier()->accept(this, indentation + 1);
    node->get_index()->accept(this, indentation + 1);
}

void PrintVisitor::visit(UnaryOpNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "UnaryOpNode " << token_values[node->get_op()]
              << std::endl;

    node->get_operand()->accept(this, indentation + 1);
}

void PrintVisitor::visit(ArrayLiteralNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "ArrayLiteralNode" << std::endl;

    for (auto &element : *node->get_values()) {
        element->accept(this, indentation + 1);
    }
}

void PrintVisitor::visit(RangeLiteralNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "RangeLiteralNode" << std::endl;

    node->get_start()->accept(this, indentation + 1);
    node->get_end()->accept(this, indentation + 1);
}

void PrintVisitor::visit(AssignmentNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "AssignmentNode" << std::endl;

    node->get_identifier()->accept(this, indentation + 1);
    node->get_value()->accept(this, indentation + 1);
}

void PrintVisitor::visit(BreakStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "BreakStatementNode" << std::endl;
}

void PrintVisitor::visit(ContinueStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "ContinueStatementNode" << std::endl;
}

void PrintVisitor::visit(ReturnStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "ReturnStatementNode" << std::endl;

    if (node->has_value()) {
        node->get_value()->accept(this, indentation + 1);
    }
}

void PrintVisitor::visit(ForStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "ForStatementNode" << std::endl;
    std::cout << std::string(indentation + 1, '\t') << node->get_identifier() << std::endl;

    node->get_iterable()->accept(this, indentation + 1);
    node->get_body()->accept(this, indentation + 1);
}

void PrintVisitor::visit(IfStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "IfStatementNode" << std::endl;

    node->get_condition()->accept(this, indentation + 1);
    node->get_if_body()->accept(this, indentation + 1);
    if (node->get_else_body() != nullptr) {
        node->get_else_body()->accept(this, indentation + 1);
    }
}

void PrintVisitor::visit(RepeatStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "RepeatStatementNode" << std::endl;

    node->get_count()->accept(this, indentation + 1);
    node->get_body()->accept(this, indentation + 1);
}

void PrintVisitor::visit(WhileStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "WhileStatementNode" << std::endl;

    node->get_condition()->accept(this, indentation + 1);
    node->get_body()->accept(this, indentation + 1);
}

void PrintVisitor::visit(FunctionDeclarationNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "FunctionDeclarationNode" << std::endl;
    std::cout << std::string(indentation + 1, '\t') << std::endl;
    for (auto &param : *node->get_parameters()) {
        std::cout << std::string(indentation + 1, '\t') << param << std::endl;
    }

    node->get_body()->accept(this, indentation + 1);
}

void PrintVisitor::visit(CallNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "CallNode" << std::endl;
    std::cout << std::string(indentation + 1, '\t') << node->get_identifier() << std::endl;

    for (auto &param : *node->get_arguments()) {
        param->accept(this, indentation + 1);
    }
}

void PrintVisitor::visit(CompoundStatementNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "CompoundStatementNode" << std::endl;

    for (auto &statement : *node->get_statements()) {
        statement->accept(this, indentation + 1);
    }
}

void PrintVisitor::visit(IdentifierNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "IdentifierNode "
              << "'" << node->get_name() << "'" << std::endl;
}

void PrintVisitor::visit(LiteralNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "LiteralNode "
              << "'" << node->get_value() << "'" << std::endl;
}

void PrintVisitor::visit(ErrorNode *node, int indentation) {
    std::cout << std::string(indentation, '\t') << "ErrorNode" << std::endl;
}
