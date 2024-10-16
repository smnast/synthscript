#include "visitor/interpreter_visitor.h"
#include "AST/AST_nodes.h"
#include "error.h"
#include "object/array_object.h"
#include "object/bool_object.h"
#include "object/float_object.h"
#include "object/function_object.h"
#include "object/string_object.h"
#include "object/void_object.h"
#include "operators.h"

std::shared_ptr<Object> InterpreterVisitor::visit(ProgramNode *node, SymbolTable *arg) {
    auto *global_table = new SymbolTable(nullptr, false, false);
    BuiltinFunctions::register_built_in_functions(global_table);
    for (auto &statement : *node->get_statements()) {
        statement->evaluate(this, global_table);
    }
    delete global_table;
    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BinOpNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> left = node->get_left_node()->evaluate(this, arg);
    std::shared_ptr<Object> right = node->get_right_node()->evaluate(this, arg);
    std::shared_ptr<Object> result = get_binary_op_function(node->get_op())(left, right);
    if (result == nullptr) {
        Error::runtime_error("Invalid operands to binary operator " + token_names[node->get_op()] + " (" + type_strings[left->get_type()] + " and " + type_strings[right->get_type()] + ")", node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CastOpNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> operand = node->get_operand()->evaluate(this, arg);
    std::shared_ptr<Object> result = operand->cast(node->get_type());

    if (result == nullptr) {
        Error::runtime_error("Invalid cast from " + type_strings[operand->get_type()] + " to " + type_strings[node->get_type()], node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(SubscriptOpNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> identifier = node->get_identifier()->evaluate(this, arg);
    std::shared_ptr<Object> index = node->get_index()->evaluate(this, arg);
    std::shared_ptr<Object> result = identifier->subscript(index);

    if (result == nullptr) {
        Error::runtime_error("Invalid subscript operation on " + type_strings[identifier->get_type()], node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(UnaryOpNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> operand = node->get_operand()->evaluate(this, arg);
    std::shared_ptr<Object> result = get_unary_op_function(node->get_op())(operand);

    if (result == nullptr) {
        Error::runtime_error("Invalid operand to unary operator " + token_names[node->get_op()] + " (" + type_strings[operand->get_type()] + ")", node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ArrayLiteralNode *node, SymbolTable *arg) {
    std::vector<std::shared_ptr<Object>> values;
    for (auto &element : *node->get_values()) {
        values.push_back(element->evaluate(this, arg));
    }
    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(RangeLiteralNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> start = node->get_start()->evaluate(this, arg);
    std::shared_ptr<Object> end = node->get_end()->evaluate(this, arg);
    if (start->get_type() != TYPE_INT) {
        Error::runtime_error("Invalid type for start of range (expected int, got " + type_strings[start->get_type()] + ")", node->get_start()->get_line(), node->get_start()->get_column());
    } else if (end->get_type() != TYPE_INT) {
        Error::runtime_error("Invalid type for end of range (expected int, got " + type_strings[start->get_type()] + ")", node->get_end()->get_line(), node->get_end()->get_column());
    }

    int start_val = std::static_pointer_cast<IntObject>(start)->get_value();
    int end_val = std::static_pointer_cast<IntObject>(end)->get_value();
    std::vector<std::shared_ptr<Object>> values;

    int dir = (start_val < end_val) ? 1 : -1;
    bool equal = false;
    int cur_val = start_val;
    while (!equal) {
        equal = cur_val == end_val;
        values.push_back(std::make_shared<IntObject>(cur_val));
        cur_val += dir;
    }

    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(AssignmentNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> value = node->get_value()->evaluate(this, arg);
    if (value->get_type() == TYPE_VOID) {
        Error::runtime_error("Invalid assignment to void", node->get_line(), node->get_column());
    }

    if (dynamic_cast<IdentifierNode *>(node->get_identifier()) != nullptr) {
        std::string name = dynamic_cast<IdentifierNode *>(node->get_identifier())->get_name();
        if (!arg->contains(name, false)) {
            Symbol symbol(name, value);
            arg->insert(symbol);
        } else {
            Symbol *symbol = arg->lookup(name, false);
            symbol->set_value(value);
        }
    } else if (dynamic_cast<SubscriptOpNode *>(node->get_identifier()) != nullptr) {
        auto *left = dynamic_cast<SubscriptOpNode *>(node->get_identifier());
        std::shared_ptr<ArrayObject> identifier = std::static_pointer_cast<ArrayObject>(left->get_identifier()->evaluate(this, arg));
        std::shared_ptr<Object> index = left->get_index()->evaluate(this, arg);
        identifier->subscript_update(index, value);
    }

    return value;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BreakStatementNode *node, SymbolTable *arg) {
    backtracking = true;
    breaking = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ContinueStatementNode *node, SymbolTable *arg) {
    backtracking = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ReturnStatementNode *node, SymbolTable *arg) {
    if (node->get_value() != nullptr) {
        return_val = node->get_value()->evaluate(this, arg);
    } else {
        return_val = std::make_shared<VoidObject>();
    }
    returning = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ForStatementNode *node, SymbolTable *arg) {
    std::string identifier = node->get_identifier();
    std::shared_ptr<Object> iterable = node->get_iterable()->evaluate(this, arg);

    int iterable_len = 0;
    if (iterable->get_type() == TYPE_ARRAY) {
        iterable_len = (int)std::static_pointer_cast<ArrayObject>(iterable)->get_value()->size();
    } else if (iterable->get_type() == TYPE_STRING) {
        iterable_len = (int)std::static_pointer_cast<StringObject>(iterable)->get_value().size();
    } else {
        Error::runtime_error("Invalid type for iterable (expected array or string, got " + type_strings[iterable->get_type()] + ")", node->get_iterable()->get_line(), node->get_iterable()->get_column());
    }

    auto *for_loop_scope = new SymbolTable(arg, true, arg->is_function());
    for_loop_scope->insert(Symbol(identifier));

    for (int i = 0; i < iterable_len; i++) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        Symbol *iterator_symbol = for_loop_scope->lookup(identifier, false);
        iterator_symbol->set_value(iterable->subscript(std::make_shared<IntObject>(i)));
        node->get_body()->evaluate(this, for_loop_scope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IfStatementNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> condition = node->get_condition()->evaluate(this, arg);
    if (condition->get_type() != TYPE_BOOL) {
        Error::runtime_error("Invalid type for if condition (expected bool, got " + type_strings[condition->get_type()] + ")", node->get_condition()->get_line(), node->get_condition()->get_column());
    } else if (std::static_pointer_cast<BoolObject>(condition)->get_value()) {
        node->get_if_body()->evaluate(this, arg);
        return nullptr;
    } else if (node->get_else_body() != nullptr) {
        node->get_else_body()->evaluate(this, arg);
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(RepeatStatementNode *node, SymbolTable *arg) {
    auto *repeat_loop_scope = new SymbolTable(arg, true, arg->is_function());
    std::shared_ptr<Object> count = node->get_count()->evaluate(this, arg);
    if (count->get_type() != TYPE_INT) {
        Error::runtime_error("Invalid type for repeat count (expected int, got " + type_strings[count->get_type()] + ")", node->get_count()->get_line(), node->get_count()->get_column());
    }

    for (int i = 0; i < std::static_pointer_cast<IntObject>(count)->get_value(); i++) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->get_body()->evaluate(this, repeat_loop_scope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(WhileStatementNode *node, SymbolTable *arg) {
    auto *while_loop_scope = new SymbolTable(arg, true, arg->is_function());
    std::shared_ptr<Object> condition = node->get_condition()->evaluate(this, arg);
    if (condition->get_type() != TYPE_BOOL) {
        Error::runtime_error("Invalid type for while condition (expected bool, got " + type_strings[condition->get_type()] + ")", node->get_condition()->get_line(), node->get_condition()->get_column());
    }

    while (std::static_pointer_cast<BoolObject>(condition)->get_value()) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->get_body()->evaluate(this, while_loop_scope);
        condition = node->get_condition()->evaluate(this, arg);

        if (condition->get_type() != TYPE_BOOL) {
            Error::runtime_error("Invalid type for while condition (expected bool, got " + type_strings[condition->get_type()] + ")", node->get_condition()->get_line(), node->get_condition()->get_column());
        }
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionDeclarationNode *node, SymbolTable *arg) {
    std::shared_ptr<Object> function_object = std::make_shared<FunctionObject>(node->get_body(), *node->get_parameters());
    Symbol function_symbol(node->get_identifier(), function_object);
    arg->insert(function_symbol);

    auto *function_scope = new SymbolTable(arg, arg->is_loop(), true);
    for (auto &param : *node->get_parameters()) {
        function_scope->insert(Symbol(param));
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionStatementNode *node, SymbolTable *arg) {
    std::string name = node->get_identifier();
    Symbol *function_symbol = arg->lookup(name, false);
    std::shared_ptr<FunctionObject> function_object = std::static_pointer_cast<FunctionObject>(function_symbol->get_value());

    if (function_object->is_built_in()) {
        std::vector<std::shared_ptr<Object>> arguments;
        for (auto &argument : *node->get_arguments()) {
            arguments.push_back(argument->evaluate(this, arg));
        }
        return BuiltinFunctions::handle_built_in_function(name, &arguments, node->get_line(), node->get_column());
    }

    return_val = std::make_shared<VoidObject>();

    auto *function_scope = new SymbolTable(arg->get_global_scope(), false, true);
    for (int i = 0; i < (int)node->get_arguments()->size(); i++) {
        std::string arg_name = function_object->get_parameters()->at(i);
        std::shared_ptr<Object> arg_value = node->get_arguments()->at(i)->evaluate(this, arg);
        function_scope->insert(Symbol(arg_name, arg_value));
    }

    function_object->get_body()->evaluate(this, function_scope);
    returning = false;
    return return_val;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CompoundStatementNode *node, SymbolTable *arg) {
    auto *scope = new SymbolTable(arg, arg->is_loop(), arg->is_function());
    for (auto &statement : *node->get_statements()) {
        if (backtracking) {
            return nullptr;
        }
        statement->evaluate(this, scope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IdentifierNode *node, SymbolTable *arg) {
    std::string name = node->get_name();
    return arg->lookup(name, false)->get_value();
}

std::shared_ptr<Object> InterpreterVisitor::visit(LiteralNode *node, SymbolTable *arg) {
    switch (node->get_type()) {
    case TYPE_INT:
        return std::make_shared<IntObject>(std::stoi(node->get_value()));
    case TYPE_FLOAT:
        return std::make_shared<FloatObject>(std::stof(node->get_value()));
    case TYPE_BOOL:
        return std::make_shared<BoolObject>(node->get_value() == "true");
    case TYPE_STRING:
        return std::make_shared<StringObject>(node->get_value().substr(1, node->get_value().length() - 2));
    default:
        return nullptr;
    }
}
