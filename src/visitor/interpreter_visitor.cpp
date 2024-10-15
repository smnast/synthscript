#include "visitor/interpreter_visitor.h"
#include "AST/AST_nodes.h"
#include "error.h"
#include "object/function_object.h"
#include "operators.h"
#include "object/array_object.h"
#include "object/bool_object.h"
#include "object/string_object.h"
#include "object/float_object.h"
#include "object/void_object.h"

std::shared_ptr<Object> InterpreterVisitor::visit(ProgramNode *node, SymbolTable* arg) {
    auto *globalTable = new SymbolTable(nullptr, false, false);
    BuiltinFunctions::registerBuiltinFunctions(globalTable);
    for (auto &statement : *node->get_statements()) {
        statement->evaluate(this, globalTable);
    }
    delete globalTable;
    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BinOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> left = node->get_left_node()->evaluate(this, arg);
    std::shared_ptr<Object> right = node->get_right_node()->evaluate(this, arg);
    std::shared_ptr<Object> result = get_binary_op_function(node->get_op())(left, right);
    if (result == nullptr) {
        Error::runtime_error("Invalid operands to binary operator " + tokenNames[node->get_op()] + " ("
                            + typeStrings[left->get_type()] + " and " + typeStrings[right->get_type()] + ")",
                            node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CastOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> operand = node->get_operand()->evaluate(this, arg);
    std::shared_ptr<Object> result = operand->cast(node->get_type());

    if (result == nullptr) {
        Error::runtime_error("Invalid cast from " + typeStrings[operand->get_type()] + " to " + typeStrings[node->get_type()],
                            node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(SubscriptOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> identifier = node->get_identifier()->evaluate(this, arg);
    std::shared_ptr<Object> index = node->get_index()->evaluate(this, arg);
    std::shared_ptr<Object> result = identifier->subscript(index);

    if (result == nullptr) {
        Error::runtime_error("Invalid subscript operation on " + typeStrings[identifier->get_type()],
                            node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(UnaryOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> operand = node->get_operand()->evaluate(this, arg);
    std::shared_ptr<Object> result = get_unary_op_function(node->get_op())(operand);

    if (result == nullptr) {
        Error::runtime_error("Invalid operand to unary operator " + tokenNames[node->get_op()] + " (" + typeStrings[operand->get_type()] + ")",
                            node->get_line(), node->get_column());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ArrayLiteralNode *node, SymbolTable* arg) {
    std::vector<std::shared_ptr<Object>> values;
    for (auto &element : *node->get_values()) {
        values.push_back(element->evaluate(this, arg));
    }
    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(RangeLiteralNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> start = node->get_start()->evaluate(this, arg);
    std::shared_ptr<Object> end = node->get_end()->evaluate(this, arg);
    if (start->get_type() != TYPE_INT) {
        Error::runtime_error("Invalid type for start of range (expected int, got " + typeStrings[start->get_type()] + ")",
                            node->get_start()->get_line(), node->get_start()->get_column());
    } else if (end->get_type() != TYPE_INT) {
        Error::runtime_error("Invalid type for end of range (expected int, got " + typeStrings[start->get_type()] + ")",
                            node->get_end()->get_line(), node->get_end()->get_column());
    }

    int startVal = std::static_pointer_cast<IntObject>(start)->get_value();
    int endVal = std::static_pointer_cast<IntObject>(end)->get_value();
    std::vector<std::shared_ptr<Object>> values;

    int dir = (startVal < endVal) ? 1 : -1;
    bool equal = false;
    int curVal = startVal;
    while (!equal) {
        equal = curVal == endVal;
        values.push_back(std::make_shared<IntObject>(curVal));
        curVal += dir;
    }

    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(AssignmentNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> value = node->get_value()->evaluate(this, arg);
    if (value->get_type() == TYPE_VOID) {
        Error::runtime_error("Invalid assignment to void", node->get_line(), node->get_column());
    }

    if (dynamic_cast<IdentifierNode*>(node->get_identifier()) != nullptr) {
        std::string name = dynamic_cast<IdentifierNode *>(node->get_identifier())->get_name();
        if (!arg->contains(name, false)) {
            Symbol symbol(name, value);
            arg->insert(symbol);
        } else {
            Symbol *symbol = arg->lookup(name, false);
            symbol->set_value(value);
        }
    } else if (dynamic_cast<SubscriptOpNode*>(node->get_identifier()) != nullptr) {
        auto *left = dynamic_cast<SubscriptOpNode*>(node->get_identifier());
        std::shared_ptr<ArrayObject> identifier = std::static_pointer_cast<ArrayObject>(left->get_identifier()->evaluate(this, arg));
        std::shared_ptr<Object> index = left->get_index()->evaluate(this, arg);
        identifier->subscriptUpdate(index, value);
    }

    return value;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BreakStatementNode *node, SymbolTable* arg) {
    backtracking = true;
    breaking = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ContinueStatementNode *node, SymbolTable* arg) {
    backtracking = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ReturnStatementNode *node, SymbolTable* arg) {
    if (node->get_value() != nullptr) {
        returnVal = node->get_value()->evaluate(this, arg);
    } else {
        returnVal = std::make_shared<VoidObject>();
    }
    returning = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ForStatementNode *node, SymbolTable* arg) {
    std::string identifier = node->get_identifier();
    std::shared_ptr<Object> iterable = node->get_iterable()->evaluate(this, arg);

    int iterableLen = 0;
    if (iterable->get_type() == TYPE_ARRAY) {
        iterableLen = (int)std::static_pointer_cast<ArrayObject>(iterable)->get_value()->size();
    } else if (iterable->get_type() == TYPE_STRING) {
        iterableLen = (int)std::static_pointer_cast<StringObject>(iterable)->get_value().size();
    } else {
        Error::runtime_error("Invalid type for iterable (expected array or string, got " + typeStrings[iterable->get_type()] + ")",
                            node->get_iterable()->get_line(), node->get_iterable()->get_column());
    }

    auto *forLoopScope = new SymbolTable(arg, true, arg->is_function());
    forLoopScope->insert(Symbol(identifier));

    for (int i = 0; i < iterableLen; i++) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        Symbol *iteratorSymbol = forLoopScope->lookup(identifier, false);
        iteratorSymbol->set_value(iterable->subscript(std::make_shared<IntObject>(i)));
        node->get_body()->evaluate(this, forLoopScope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IfStatementNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> condition = node->get_condition()->evaluate(this, arg);
    if (condition->get_type() != TYPE_BOOL) {
        Error::runtime_error("Invalid type for if condition (expected bool, got " + typeStrings[condition->get_type()] + ")",
                            node->get_condition()->get_line(), node->get_condition()->get_column());
    } else if (std::static_pointer_cast<BoolObject>(condition)->get_value()) {
        node->get_if_body()->evaluate(this, arg);
        return nullptr;
    } else if (node->get_else_body() != nullptr) {
        node->get_else_body()->evaluate(this, arg);
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(RepeatStatementNode *node, SymbolTable* arg) {
    auto *repeatLoopScope = new SymbolTable(arg, true, arg->is_function());
    std::shared_ptr<Object> count = node->get_count()->evaluate(this, arg);
    if (count->get_type() != TYPE_INT) {
        Error::runtime_error("Invalid type for repeat count (expected int, got " + typeStrings[count->get_type()] + ")",
                            node->get_count()->get_line(), node->get_count()->get_column());
    }

    for (int i = 0; i < std::static_pointer_cast<IntObject>(count)->get_value(); i++) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->get_body()->evaluate(this, repeatLoopScope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(WhileStatementNode *node, SymbolTable* arg) {
    auto *whileLoopScope = new SymbolTable(arg, true, arg->is_function());
    std::shared_ptr<Object> condition = node->get_condition()->evaluate(this, arg);
    if (condition->get_type() != TYPE_BOOL) {
        Error::runtime_error("Invalid type for while condition (expected bool, got " + typeStrings[condition->get_type()] + ")",
                            node->get_condition()->get_line(), node->get_condition()->get_column());
    }

    while (std::static_pointer_cast<BoolObject>(condition)->get_value()) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->get_body()->evaluate(this, whileLoopScope);
        condition = node->get_condition()->evaluate(this, arg);

        if (condition->get_type() != TYPE_BOOL) {
            Error::runtime_error("Invalid type for while condition (expected bool, got " + typeStrings[condition->get_type()] + ")",
                                node->get_condition()->get_line(), node->get_condition()->get_column());
        }
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionDeclarationNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> functionObject = std::make_shared<FunctionObject>(node->get_body(), *node->get_parameters());
    Symbol functionSymbol(node->get_identifier(), functionObject);
    arg->insert(functionSymbol);

    auto *functionScope = new SymbolTable(arg, arg->is_loop(), true);
    for (auto &param : *node->get_parameters()) {
        functionScope->insert(Symbol(param));
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionStatementNode *node, SymbolTable* arg) {
    std::string name = node->get_identifier();
    Symbol *functionSymbol = arg->lookup(name, false);
    std::shared_ptr<FunctionObject> functionObject = std::static_pointer_cast<FunctionObject>(functionSymbol->get_value());

    if (functionObject->is_built_in()) {
        std::vector<std::shared_ptr<Object>> arguments;
        for (auto &argument : *node->get_arguments()) {
            arguments.push_back(argument->evaluate(this, arg));
        }
        return BuiltinFunctions::handleBuiltinFunction(name, &arguments, node->get_line(), node->get_column());
    }

    returnVal = std::make_shared<VoidObject>();

    auto *functionScope = new SymbolTable(arg->get_global_scope(), false, true);
    for (int i = 0; i < (int)node->get_arguments()->size(); i++) {
        std::string argName = functionObject->get_parameters()->at(i);
        std::shared_ptr<Object> argValue = node->get_arguments()->at(i)->evaluate(this, arg);
        functionScope->insert(Symbol(argName, argValue));
    }

    functionObject->get_body()->evaluate(this, functionScope);
    returning = false;
    return returnVal;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CompoundStatementNode *node, SymbolTable* arg) {
    auto *scope = new SymbolTable(arg, arg->is_loop(), arg->is_function());
    for (auto &statement : *node->get_statements()) {
        if (backtracking) return nullptr;
        statement->evaluate(this, scope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IdentifierNode *node, SymbolTable* arg) {
    std::string name = node->get_name();
    return arg->lookup(name, false)->get_value();
}

std::shared_ptr<Object> InterpreterVisitor::visit(LiteralNode *node, SymbolTable* arg) {
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
