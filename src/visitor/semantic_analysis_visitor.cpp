#include "visitor/semantic_analysis_visitor.h"
#include "AST/AST_nodes.h"
#include "object/function_object.h"

void SemanticAnalysisVisitor::visit(ProgramNode *node, SymbolTable* arg) {
    auto *global_table = new SymbolTable(nullptr, false, false);
    BuiltinFunctions::register_built_in_functions(global_table);

    for (auto &statement : *node->get_statements()) {
        statement->analyze(this, global_table);
    }
    delete global_table;
}

void SemanticAnalysisVisitor::visit(BinOpNode *node, SymbolTable* arg) {
    node->get_left_node()->analyze(this, arg);
    node->get_right_node()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(CastOpNode *node, SymbolTable* arg) {
    node->get_operand()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(SubscriptOpNode *node, SymbolTable* arg) {
    node->get_identifier()->analyze(this, arg);
    node->get_index()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(UnaryOpNode *node, SymbolTable* arg) {
    node->get_operand()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(ArrayLiteralNode *node, SymbolTable* arg) {
    for (auto &element : *node->get_values()) {
        element->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(RangeLiteralNode *node, SymbolTable* arg) {
    node->get_start()->analyze(this, arg);
    node->get_end()->analyze(this, arg);
}

std::string SemanticAnalysisVisitor::get_array_identifier(SubscriptOpNode *node, SymbolTable* arg) {
    auto subscript_op_node = dynamic_cast<SubscriptOpNode*>(node->get_identifier());
    if (subscript_op_node != nullptr) {
        return get_array_identifier(subscript_op_node, arg);
    } else {
        return dynamic_cast<IdentifierNode*>(node->get_identifier())->get_name();
    }
}

void SemanticAnalysisVisitor::visit(AssignmentNode *node, SymbolTable* arg) {
    if (dynamic_cast<SubscriptOpNode*>(node->get_identifier()) != nullptr) {
        auto *subscript_op_node = dynamic_cast<SubscriptOpNode*>(node->get_identifier());
        std::string name = get_array_identifier(subscript_op_node, arg);
        if (!arg->contains(name, false)) {
            semantic_error("Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
        }
    } else if (dynamic_cast<IdentifierNode*>(node->get_identifier()) != nullptr) {
        std::string name = dynamic_cast<IdentifierNode*>(node->get_identifier())->get_name();
        if (!arg->contains(name, false)) {
            arg->insert(Symbol(name));
        }
    } else {
        semantic_error("Invalid assignment operand", node->get_line(), node->get_column());
    }
    node->get_identifier()->analyze(this, arg);
    node->get_value()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(BreakStatementNode *node, SymbolTable* arg) {
    if (!arg->is_loop()) {
        semantic_error("Break statement outside of loop", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(ContinueStatementNode *node, SymbolTable* arg) {
    if (!arg->is_loop()) {
        semantic_error("Continue statement outside of loop", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(ReturnStatementNode *node, SymbolTable* arg) {
    if (!arg->is_function()) {
        semantic_error("Return statement outside of function", node->get_line(), node->get_column());
    }
    if (node->get_value() != nullptr) {
        node->get_value()->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(ForStatementNode *node, SymbolTable* arg) {
    std::string identifier = node->get_identifier();
    node->get_iterable()->analyze(this, arg);

    auto *for_loop_scope = new SymbolTable(arg, true, arg->is_function());
    for_loop_scope->insert(Symbol(identifier));
    node->get_body()->analyze(this, for_loop_scope);
}

void SemanticAnalysisVisitor::visit(IfStatementNode *node, SymbolTable* arg) {
    node->get_condition()->analyze(this, arg);

    node->get_if_body()->analyze(this, arg);
    if (node->get_else_body() != nullptr) {
        node->get_else_body()->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(RepeatStatementNode *node, SymbolTable* arg) {
    auto *repeat_loop_scope = new SymbolTable(arg, true, arg->is_function());
    node->get_count()->analyze(this, repeat_loop_scope);
    node->get_body()->analyze(this, repeat_loop_scope);
}

void SemanticAnalysisVisitor::visit(WhileStatementNode *node, SymbolTable* arg) {
    auto *while_loop_scope = new SymbolTable(arg, true, arg->is_function());
    node->get_condition()->analyze(this, while_loop_scope);
    node->get_body()->analyze(this, while_loop_scope);
}

void SemanticAnalysisVisitor::visit(FunctionDeclarationNode *node, SymbolTable* arg) {
    if (!arg->is_global_scope()) {
        semantic_error("Function declaration outside of global scope", node->get_line(), node->get_column());
    }

    if (arg->contains(node->get_identifier(), false)) {
        semantic_error("Identifier '" + node->get_identifier() + "' already declared", node->get_line(), node->get_column());
    }

    std::shared_ptr<Object> function_object = std::make_shared<FunctionObject>(node->get_body(), *node->get_parameters());
    Symbol function_symbol(node->get_identifier(), function_object);
    arg->insert(function_symbol);

    auto *function_scope = new SymbolTable(arg, arg->is_loop(), true);
    for (auto &param : *node->get_parameters()) {
        function_scope->insert(Symbol(param));
    }

    node->get_body()->analyze(this, function_scope);
}

void SemanticAnalysisVisitor::visit(FunctionStatementNode *node, SymbolTable* arg) {
    std::string name = node->get_identifier();

    if (!arg->contains(name, false)) {
        semantic_error("Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
    }
    for (auto &param : *node->get_arguments()) {
        param->analyze(this, arg);
    }

    Symbol *function_symbol = arg->lookup(name, false);
    if (function_symbol->get_type() != TYPE_FUNCTION) {
        semantic_error("Identifier '" + name + "' is not a function", node->get_line(), node->get_column());
    } else {
        FunctionObject *function_object = dynamic_cast<FunctionObject*>(function_symbol->get_value().get());
        if (function_object->get_parameters()->size() != node->get_arguments()->size()) {
            semantic_error("Incorrect number of arguments to function '" + name + "'", node->get_line(), node->get_column());
        }
    }
}

void SemanticAnalysisVisitor::visit(CompoundStatementNode *node, SymbolTable* arg) {
    auto *scope = new SymbolTable(arg, arg->is_loop(), arg->is_function());
    for (auto &statement : *node->get_statements()) {
        statement->analyze(this, scope);
    }
}

void SemanticAnalysisVisitor::visit(IdentifierNode *node, SymbolTable* arg) {
    std::string name = node->get_name();
    if (!arg->contains(name, false)) {
        semantic_error("Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
    } else if (arg->lookup(name, false)->get_type() == TYPE_FUNCTION) {
        semantic_error("Identifier '" + name + "' is a function", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(LiteralNode *node, SymbolTable* arg) {
}

void SemanticAnalysisVisitor::semantic_error(const std::string &message, int line, int column) {
    Error::error_at_pos(message, line, column, true);
}
