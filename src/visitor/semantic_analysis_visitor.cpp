#include "visitor/semantic_analysis_visitor.h"
#include "AST/AST_nodes.h"
#include "built_in_functions.h"
#include "object/function_object.h"

SemanticAnalysisVisitor::SemanticAnalysisVisitor(ProgramNode *program_node,
                                                 ErrorManager *error_manager)
    : program_node(program_node), error_manager(error_manager), built_in_functions(error_manager) {}

void SemanticAnalysisVisitor::analyze() {
    program_node->analyze(this, nullptr);
}

void SemanticAnalysisVisitor::visit(ProgramNode *node, SymbolTable *table) {
    auto *global_table = new SymbolTable(nullptr, false, false);
    built_in_functions.register_built_in_functions(global_table);

    for (auto &statement : *node->get_statements()) {
        statement->analyze(this, global_table);
    }

    delete global_table;
}

void SemanticAnalysisVisitor::visit(BinOpNode *node, SymbolTable *table) {
    node->get_left_node()->analyze(this, table);
    node->get_right_node()->analyze(this, table);
}

void SemanticAnalysisVisitor::visit(CastOpNode *node, SymbolTable *table) {
    node->get_operand()->analyze(this, table);
}

void SemanticAnalysisVisitor::visit(SubscriptOpNode *node, SymbolTable *table) {
    node->get_identifier()->analyze(this, table);
    node->get_index()->analyze(this, table);
}

void SemanticAnalysisVisitor::visit(UnaryOpNode *node, SymbolTable *table) {
    node->get_operand()->analyze(this, table);
}

void SemanticAnalysisVisitor::visit(ArrayLiteralNode *node, SymbolTable *table) {
    for (auto &element : *node->get_values()) {
        element->analyze(this, table);
    }
}

void SemanticAnalysisVisitor::visit(RangeLiteralNode *node, SymbolTable *table) {
    node->get_start()->analyze(this, table);
    node->get_end()->analyze(this, table);
}

void SemanticAnalysisVisitor::visit(AssignmentNode *node, SymbolTable *table) {
    // Check if the identifier is an array subscript operation
    if (node->get_identifier()->get_node_type() == NodeType::SUBSCRIPT_OP_NODE) {
        auto subscript_op_node = static_cast<SubscriptOpNode *>(node->get_identifier());
        std::string name = get_array_identifier(subscript_op_node, table);
        if (!table->contains(name, false)) {
            semantic_error(
                "Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
        }
    }
    // Check if the identifier is just an identifier
    else if (node->get_identifier()->get_node_type() == NodeType::IDENTIFIER_NODE) {
        auto identifier_node = static_cast<IdentifierNode *>(node->get_identifier());
        std::string name = identifier_node->get_name();
        if (!table->contains(name, false)) {
            table->insert(Symbol(name));
        }
    } else {
        semantic_error("Invalid assignment operand", node->get_line(), node->get_column());
    }

    node->get_identifier()->analyze(this, table);
    node->get_value()->analyze(this, table);
}

void SemanticAnalysisVisitor::visit(BreakStatementNode *node, SymbolTable *table) {
    if (!table->is_loop()) {
        semantic_error(token_values[BREAK_KEYWORD] + " statement outside of loop",
                       node->get_line(),
                       node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(ContinueStatementNode *node, SymbolTable *table) {
    if (!table->is_loop()) {
        semantic_error(token_values[CONTINUE_KEYWORD] + " statement outside of loop",
                       node->get_line(),
                       node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(ReturnStatementNode *node, SymbolTable *table) {
    if (!table->is_function()) {
        semantic_error(token_values[RETURN_KEYWORD] + " statement outside of function",
                       node->get_line(),
                       node->get_column());
    }

    if (node->has_value()) {
        node->get_value()->analyze(this, table);
    }
}

void SemanticAnalysisVisitor::visit(ForStatementNode *node, SymbolTable *table) {
    std::string identifier = node->get_identifier();
    node->get_iterable()->analyze(this, table);

    // New scope for the for loop (includes the identifier)
    auto *for_loop_table = new SymbolTable(table, true, table->is_function());
    for_loop_table->insert(Symbol(identifier));

    node->get_body()->analyze(this, for_loop_table);
}

void SemanticAnalysisVisitor::visit(IfStatementNode *node, SymbolTable *table) {
    // New scope for the if statement (includes anything in the condition as well)
    auto *if_statement_table = new SymbolTable(table, table->is_loop(), table->is_function());

    node->get_condition()->analyze(this, if_statement_table);
    node->get_if_body()->analyze(this, if_statement_table);

    if (node->get_else_body() != nullptr) {
        node->get_else_body()->analyze(this, table);
    }
}

void SemanticAnalysisVisitor::visit(RepeatStatementNode *node, SymbolTable *table) {
    // New scope for the repeat loop
    auto *repeat_loop_table = new SymbolTable(table, true, table->is_function());

    node->get_count()->analyze(this, repeat_loop_table);
    node->get_body()->analyze(this, repeat_loop_table);
}

void SemanticAnalysisVisitor::visit(WhileStatementNode *node, SymbolTable *table) {
    // New scope for the while loop
    auto *while_loop_table = new SymbolTable(table, true, table->is_function());

    node->get_condition()->analyze(this, while_loop_table);
    node->get_body()->analyze(this, while_loop_table);
}

void SemanticAnalysisVisitor::visit(FunctionDeclarationNode *node, SymbolTable *table) {
    // Insert the function into the symbol table
    std::shared_ptr<Object> function_object =
        std::make_shared<FunctionObject>(node->get_body(), *node->get_parameters());

    // New scope for the function (includes the function's parameters)
    auto *function_table = new SymbolTable(table, table->is_loop(), true);
    for (auto &param : *node->get_parameters()) {
        function_table->insert(Symbol(param));
    }

    node->get_body()->analyze(this, function_table);
}

void SemanticAnalysisVisitor::visit(CallOpNode *node, SymbolTable *table) {
    std::string name = node->get_identifier();

    // The function must be declared before it is called
    bool function_exists = table->contains(name, false);
    if (!function_exists) {
        semantic_error(
            "Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
    }

    for (auto &param : *node->get_arguments()) {
        param->analyze(this, table);
    }
}

void SemanticAnalysisVisitor::visit(CompoundStatementNode *node, SymbolTable *table) {
    // New scope for the compound statement
    auto *compound_statement_table = new SymbolTable(table, table->is_loop(), table->is_function());

    for (auto &statement : *node->get_statements()) {
        statement->analyze(this, compound_statement_table);
    }
}

void SemanticAnalysisVisitor::visit(IdentifierNode *node, SymbolTable *table) {
    std::string name = node->get_name();

    // Check if the identifier was declared
    if (!table->contains(name, false)) {
        semantic_error(
            "Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
    }
    // Check if the identifier is a function (functions cannot be used as normal variables)
    else if (table->get(name, false)->get_type() == TYPE_FUNCTION) {
        semantic_error(
            "Identifier '" + name + "' is a function", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(LiteralNode *node, SymbolTable *table) {
    // Literals do not need to be analyzed
}

void SemanticAnalysisVisitor::visit(ErrorNode *node, SymbolTable *table) {
    // Errors do not need to be analyzed (they already prevent execution)
}

std::string SemanticAnalysisVisitor::get_array_identifier(SubscriptOpNode *node,
                                                          SymbolTable *table) {
    // Check if the array is nested
    if (node->get_identifier()->get_node_type() == NodeType::SUBSCRIPT_OP_NODE) {
        auto subscript_op_node = static_cast<SubscriptOpNode *>(node->get_identifier());
        return get_array_identifier(subscript_op_node, table);
    }
    // Otherwise, it is just an identifier
    else if (node->get_identifier()->get_node_type() == NodeType::IDENTIFIER_NODE) {
        return static_cast<IdentifierNode *>(node->get_identifier())->get_name();
    } else {
        semantic_error("Invalid subscript operation", node->get_line(), node->get_column());
        return "";
    }
}

void SemanticAnalysisVisitor::semantic_error(const std::string &message, int line, int column) {
    error_manager->error_at_pos(message, line, column, true);
}
