#include "visitor/interpreter_visitor.h"
#include "AST/AST_nodes.h"
#include "built_in_functions.h"
#include "object/array_object.h"
#include "object/bool_object.h"
#include "object/float_object.h"
#include "object/function_object.h"
#include "object/int_object.h"
#include "object/string_object.h"
#include "object/void_object.h"
#include "operators.h"
#include <stdexcept>

InterpreterVisitor::InterpreterVisitor(ProgramNode *program_node, ErrorManager *error_manager)
    : program_node(program_node), error_manager(error_manager), built_in_functions(error_manager) {}

void InterpreterVisitor::interpret() {
    program_node->evaluate(this, nullptr);
}

std::shared_ptr<Object> InterpreterVisitor::visit(ProgramNode *node, SymbolTable *table) {
    // Create symbol table for the global scope
    auto *global_table = new SymbolTable(nullptr, false, false);

    built_in_functions.register_built_in_functions(global_table);

    for (auto &statement : *node->get_statements()) {
        statement->evaluate(this, global_table);
    }

    delete global_table;
    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BinOpNode *node, SymbolTable *table) {
    std::shared_ptr<Object> left = node->get_left_node()->evaluate(this, table);
    std::shared_ptr<Object> right = node->get_right_node()->evaluate(this, table);
    std::shared_ptr<Object> result = get_binary_op_function(node->get_op())(left, right);

    // nullptr result indicates an invalid operation
    if (result == nullptr) {
        runtime_error("Invalid operands to binary operator " + token_values[node->get_op()] + " (" +
                          type_to_string(left->get_type()) + " and " +
                          type_to_string(right->get_type()) + ")",
                      node->get_line(),
                      node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CastOpNode *node, SymbolTable *table) {
    std::shared_ptr<Object> operand = node->get_operand()->evaluate(this, table);
    std::shared_ptr<Object> result = operand->cast(node->get_type());

    // nullptr result indicates an invalid operation
    if (result == nullptr) {
        runtime_error("Invalid cast from " + type_to_string(operand->get_type()) + " to " +
                          type_to_string(node->get_type()),
                      node->get_line(),
                      node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(SubscriptOpNode *node, SymbolTable *table) {
    std::shared_ptr<Object> identifier = node->get_identifier()->evaluate(this, table);
    std::shared_ptr<Object> index = node->get_index()->evaluate(this, table);
    std::shared_ptr<Object> result = identifier->subscript(index);

    // nullptr result indicates an invalid operation
    if (result == nullptr) {
        runtime_error("Invalid subscript operation on " + type_to_string(identifier->get_type()),
                      node->get_line(),
                      node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(UnaryOpNode *node, SymbolTable *table) {
    std::shared_ptr<Object> operand = node->get_operand()->evaluate(this, table);
    std::shared_ptr<Object> result = get_unary_op_function(node->get_op())(operand);

    // nullptr result indicates an invalid operation
    if (result == nullptr) {
        runtime_error("Invalid operand to unary operator " + token_values[node->get_op()] + " (" +
                          type_to_string(operand->get_type()) + ")",
                      node->get_line(),
                      node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ArrayLiteralNode *node, SymbolTable *table) {
    // Create an array object from the values
    std::vector<std::shared_ptr<Object>> values;
    for (auto &element : *node->get_values()) {
        values.push_back(element->evaluate(this, table));
    }

    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(RangeLiteralNode *node, SymbolTable *table) {
    std::shared_ptr<Object> start = node->get_start()->evaluate(this, table);
    std::shared_ptr<Object> end = node->get_end()->evaluate(this, table);

    // The start value must be an integer
    if (start->get_type() != TYPE_INT) {
        runtime_error("Invalid type for start of range (expected int, got " +
                          type_to_string(start->get_type()) + ")",
                      node->get_start()->get_line(),
                      node->get_start()->get_column());
    }
    // The end value must be an integer
    else if (end->get_type() != TYPE_INT) {
        runtime_error("Invalid type for end of range (expected int, got " +
                          type_to_string(start->get_type()) + ")",
                      node->get_end()->get_line(),
                      node->get_end()->get_column());
    }

    int start_val = std::static_pointer_cast<IntObject>(start)->get_value();
    int end_val = std::static_pointer_cast<IntObject>(end)->get_value();
    std::vector<std::shared_ptr<Object>> values;

    // Iterate from start to end (inclusive of both) and add each value to the array
    int dir = (start_val < end_val) ? 1 : -1;
    bool last_equal = false;
    int cur_val = start_val;

    // Iterate and add values
    while (!last_equal) {
        last_equal = cur_val == end_val;
        values.push_back(std::make_shared<IntObject>(cur_val));
        cur_val += dir;
    }

    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(AssignmentNode *node, SymbolTable *table) {
    std::shared_ptr<Object> value = node->get_value()->evaluate(this, table);

    // Cannot assign to void
    if (value->get_type() == TYPE_VOID) {
        runtime_error("Invalid assignment to void", node->get_line(), node->get_column());
    }

    // If the identifier is an array subscript operation
    if (auto left = dynamic_cast<SubscriptOpNode *>(node->get_identifier())) {
        // Evaluate the expression on the left
        std::shared_ptr<ArrayObject> identifier =
            std::static_pointer_cast<ArrayObject>(left->get_identifier()->evaluate(this, table));

        // Update the value at the index
        std::shared_ptr<Object> index = left->get_index()->evaluate(this, table);
        identifier->subscript_update(index, value);
    }
    // If the identifier is just an identifier
    else if (auto identifier_node = dynamic_cast<IdentifierNode *>(node->get_identifier())) {
        std::string name = identifier_node->get_name();

        if (table->contains(name, false)) {
            // Update the value of the existing symbol
            Symbol *symbol = table->get(name, false);
            symbol->set_value(value);
        } else {
            // Create a new symbol with the value
            Symbol symbol(name, value);
            table->insert(symbol);
        }
    }

    return value;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BreakStatementNode *node, SymbolTable *table) {
    backtracking = true;
    breaking = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ContinueStatementNode *node, SymbolTable *table) {
    backtracking = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ReturnStatementNode *node, SymbolTable *table) {
    // Evaluate the return value
    if (node->has_value()) {
        return_values.top() = node->get_value()->evaluate(this, table);
    }
    // No return value, so return void
    else {
        return_values.top() = std::make_shared<VoidObject>();
    }

    returning = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ForStatementNode *node, SymbolTable *table) {
    std::string identifier = node->get_identifier();

    std::shared_ptr<Object> iterable = node->get_iterable()->evaluate(this, table);
    int iterable_len = 0;

    // Iterate through an array
    if (iterable->get_type() == TYPE_ARRAY) {
        iterable_len = (int)std::static_pointer_cast<ArrayObject>(iterable)->get_value()->size();
    }
    // Iterate through a string
    else if (iterable->get_type() == TYPE_STRING) {
        iterable_len = (int)std::static_pointer_cast<StringObject>(iterable)->get_value().size();
    } else {
        runtime_error("Invalid type for iterable (expected array or string, got " +
                          type_to_string(iterable->get_type()) + ")",
                      node->get_iterable()->get_line(),
                      node->get_iterable()->get_column());
    }

    // Create a new scope for the for loop
    auto *for_loop_table = new SymbolTable(table, true, table->is_function());
    for_loop_table->insert(Symbol(identifier));

    for (int i = 0; i < iterable_len; i++) {
        // Handle breaking
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        // Set the value of the iterator
        Symbol *iterator_symbol = for_loop_table->get(identifier, false);
        iterator_symbol->set_value(iterable->subscript(std::make_shared<IntObject>(i)));

        node->get_body()->evaluate(this, for_loop_table);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IfStatementNode *node, SymbolTable *table) {
    // Create a new scope for the if statement
    SymbolTable *if_statement_table =
        new SymbolTable(table, table->is_loop(), table->is_function());

    std::shared_ptr<Object> condition = node->get_condition()->evaluate(this, if_statement_table);

    // The condition must be a boolean
    if (condition->get_type() != TYPE_BOOL) {
        runtime_error("Invalid type for if condition (expected bool, got " +
                          type_to_string(condition->get_type()) + ")",
                      node->get_condition()->get_line(),
                      node->get_condition()->get_column());
    }
    // If condition, then evaluate the if body
    else if (std::static_pointer_cast<BoolObject>(condition)->get_value()) {
        node->get_if_body()->evaluate(this, if_statement_table);
        return nullptr;
    }
    // Else, evaluate the else body (if it exists)
    else if (node->get_else_body() != nullptr) {
        node->get_else_body()->evaluate(this, if_statement_table);
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(RepeatStatementNode *node, SymbolTable *table) {
    // Create a new scope for the repeat loop
    auto *repeat_loop_table = new SymbolTable(table, true, table->is_function());

    // Count must be an integer
    std::shared_ptr<Object> count = node->get_count()->evaluate(this, table);
    if (count->get_type() != TYPE_INT) {
        runtime_error("Invalid type for repeat count (expected int, got " +
                          type_to_string(count->get_type()) + ")",
                      node->get_count()->get_line(),
                      node->get_count()->get_column());
    }

    // Repeat the body `count` times
    for (int i = 0; i < std::static_pointer_cast<IntObject>(count)->get_value(); i++) {
        // Handle breaking
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->get_body()->evaluate(this, repeat_loop_table);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(WhileStatementNode *node, SymbolTable *table) {
    // Create a new scope for the while loop
    auto *while_loop_table = new SymbolTable(table, true, table->is_function());

    // The condition must be a boolean
    std::shared_ptr<Object> condition = node->get_condition()->evaluate(this, table);
    if (condition->get_type() != TYPE_BOOL) {
        runtime_error("Invalid type for while condition (expected bool, got " +
                          type_to_string(condition->get_type()) + ")",
                      node->get_condition()->get_line(),
                      node->get_condition()->get_column());
    }

    // While the condition is true, evaluate the body
    while (std::static_pointer_cast<BoolObject>(condition)->get_value()) {
        // Handle breaking
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        // Evalulate the body
        node->get_body()->evaluate(this, while_loop_table);

        // Update the condition
        condition = node->get_condition()->evaluate(this, table);
        if (condition->get_type() != TYPE_BOOL) {
            runtime_error("Invalid type for while condition (expected bool, got " +
                              type_to_string(condition->get_type()) + ")",
                          node->get_condition()->get_line(),
                          node->get_condition()->get_column());
        }
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionDeclarationNode *node,
                                                  SymbolTable *table) {
    // Create a symbol for the function
    std::shared_ptr<Object> function_object =
        std::make_shared<FunctionObject>(node->get_body(), *node->get_parameters());

    return function_object;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CallNode *node, SymbolTable *table) {
    // Get the function object from the symbol table
    std::string name = node->get_identifier();
    Symbol *function_symbol = table->get(name, false);
    std::shared_ptr<FunctionObject> function_object =
        std::static_pointer_cast<FunctionObject>(function_symbol->get_value());

    // If the symbol is a function
    if (function_symbol->get_type() == TYPE_FUNCTION) {
        // Check if the number of arguments is correct
        if (function_object->get_parameters()->size() != node->get_arguments()->size()) {
            runtime_error("Incorrect number of arguments to function '" + name + "' (expected " +
                              std::to_string(function_object->get_parameters()->size()) +
                              ", given " + std::to_string(node->get_arguments()->size()) + ")",
                          node->get_line(),
                          node->get_column());
        }
    } else {
        runtime_error(
            "Identifier '" + name + "' is not a function", node->get_line(), node->get_column());
    }

    // Handle built-in functions
    if (function_object->is_built_in()) {
        std::vector<std::shared_ptr<Object>> arguments;
        for (auto &argument : *node->get_arguments()) {
            arguments.push_back(argument->evaluate(this, table));
        }

        return built_in_functions.handle_built_in_function(
            name, &arguments, node->get_line(), node->get_column());
    }

    // Push a new return value to the stack
    return_values.push(std::make_shared<VoidObject>());

    // Create a new scope for the function with the arguments
    auto *function_table = new SymbolTable(table->get_global_scope(), false, true);
    for (size_t i = 0; i < node->get_arguments()->size(); i++) {
        std::string arg_name = function_object->get_parameters()->at(i);
        std::shared_ptr<Object> arg_value = node->get_arguments()->at(i)->evaluate(this, table);
        function_table->insert(Symbol(arg_name, arg_value));
    }

    // Evaluate the function body
    function_object->call(this, function_table);

    returning = false;

    // Get the return value from the top of the stack
    std::shared_ptr<Object> return_value = return_values.top();
    return_values.pop();

    return return_value;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CompoundStatementNode *node, SymbolTable *table) {
    // Create a new scope for the compound statement
    auto *compound_statement_table = new SymbolTable(table, table->is_loop(), table->is_function());

    for (auto &statement : *node->get_statements()) {
        // Handle breaking
        if (backtracking || returning) {
            return nullptr;
        }

        statement->evaluate(this, compound_statement_table);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IdentifierNode *node, SymbolTable *table) {
    // Get identifier value from the symbol table
    std::string name = node->get_name();
    return table->get(name, false)->get_value();
}

std::shared_ptr<Object> InterpreterVisitor::visit(LiteralNode *node, SymbolTable *table) {
    // Create an object from the literal value
    switch (node->get_type()) {
    case TYPE_INT:
        try {
            return std::make_shared<IntObject>(node->get_value());
        } catch (const std::out_of_range &e) {
            runtime_error("Integer value out of range", node->get_line(), node->get_column());
        }
    case TYPE_FLOAT:
        try {
            return std::make_shared<FloatObject>(node->get_value());
        } catch (const std::out_of_range &e) {
            runtime_error("Float value out of range", node->get_line(), node->get_column());
        }
    case TYPE_BOOL:
        return std::make_shared<BoolObject>(node->get_value());
    case TYPE_STRING:
        return StringObject::from_string_literal(node->get_value());
    default:
        return nullptr;
    }
}

std::shared_ptr<Object> InterpreterVisitor::visit(ErrorNode *node, SymbolTable *table) {
    // This should never occur!
    runtime_error("Error node", node->get_line(), node->get_column());
    return nullptr;
}

void InterpreterVisitor::runtime_error(const std::string &message, int line, int column) {
    error_manager->runtime_error(message, line, column);
}
