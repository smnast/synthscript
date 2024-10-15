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
    for (auto &statement : *node->getStatements()) {
        statement->evaluate(this, globalTable);
    }
    delete globalTable;
    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(BinOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> left = node->getLeftNode()->evaluate(this, arg);
    std::shared_ptr<Object> right = node->getRightNode()->evaluate(this, arg);
    std::shared_ptr<Object> result = getBinaryOpFunction(node->getOp())(left, right);
    if (result == nullptr) {
        Error::runtimeError("Invalid operands to binary operator " + tokenNames[node->getOp()] + " ("
                            + typeStrings[left->getType()] + " and " + typeStrings[right->getType()] + ")",
                            node->getLineNumber(), node->getColumnNumber());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CastOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> operand = node->getOperand()->evaluate(this, arg);
    std::shared_ptr<Object> result = operand->cast(node->getType());

    if (result == nullptr) {
        Error::runtimeError("Invalid cast from " + typeStrings[operand->getType()] + " to " + typeStrings[node->getType()],
                            node->getLineNumber(), node->getColumnNumber());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(SubscriptOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> identifier = node->getIdentifier()->evaluate(this, arg);
    std::shared_ptr<Object> index = node->getIndex()->evaluate(this, arg);
    std::shared_ptr<Object> result = identifier->subscript(index);

    if (result == nullptr) {
        Error::runtimeError("Invalid subscript operation on " + typeStrings[identifier->getType()],
                            node->getLineNumber(), node->getColumnNumber());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(UnaryOpNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> operand = node->getOperand()->evaluate(this, arg);
    std::shared_ptr<Object> result = getUnaryOpFunction(node->getOp())(operand);

    if (result == nullptr) {
        Error::runtimeError("Invalid operand to unary operator " + tokenNames[node->getOp()] + " (" + typeStrings[operand->getType()] + ")",
                            node->getLineNumber(), node->getColumnNumber());
    }

    return result;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ArrayLiteralNode *node, SymbolTable* arg) {
    std::vector<std::shared_ptr<Object>> values;
    for (auto &element : *node->getValues()) {
        values.push_back(element->evaluate(this, arg));
    }
    return std::make_shared<ArrayObject>(values);
}

std::shared_ptr<Object> InterpreterVisitor::visit(RangeLiteralNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> start = node->getStart()->evaluate(this, arg);
    std::shared_ptr<Object> end = node->getEnd()->evaluate(this, arg);
    if (start->getType() != TYPE_INT) {
        Error::runtimeError("Invalid type for start of range (expected int, got " + typeStrings[start->getType()] + ")",
                            node->getStart()->getLineNumber(), node->getStart()->getColumnNumber());
    } else if (end->getType() != TYPE_INT) {
        Error::runtimeError("Invalid type for end of range (expected int, got " + typeStrings[start->getType()] + ")",
                            node->getEnd()->getLineNumber(), node->getEnd()->getColumnNumber());
    }

    int startVal = std::static_pointer_cast<IntObject>(start)->getValue();
    int endVal = std::static_pointer_cast<IntObject>(end)->getValue();
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
    std::shared_ptr<Object> value = node->getValue()->evaluate(this, arg);
    if (value->getType() == TYPE_VOID) {
        Error::runtimeError("Invalid assignment to void", node->getLineNumber(), node->getColumnNumber());
    }

    if (dynamic_cast<IdentifierNode*>(node->getIdentifier()) != nullptr) {
        std::string name = dynamic_cast<IdentifierNode *>(node->getIdentifier())->getName();
        if (!arg->contains(name, false)) {
            Symbol symbol(name, value);
            arg->insert(symbol);
        } else {
            Symbol *symbol = arg->lookup(name, false);
            symbol->setValue(value);
        }
    } else if (dynamic_cast<SubscriptOpNode*>(node->getIdentifier()) != nullptr) {
        auto *left = dynamic_cast<SubscriptOpNode*>(node->getIdentifier());
        std::shared_ptr<ArrayObject> identifier = std::static_pointer_cast<ArrayObject>(left->getIdentifier()->evaluate(this, arg));
        std::shared_ptr<Object> index = left->getIndex()->evaluate(this, arg);
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
    if (node->getValue() != nullptr) {
        returnVal = node->getValue()->evaluate(this, arg);
    } else {
        returnVal = std::make_shared<VoidObject>();
    }
    returning = true;

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(ForStatementNode *node, SymbolTable* arg) {
    std::string identifier = node->getIdentifier();
    std::shared_ptr<Object> iterable = node->getIterable()->evaluate(this, arg);

    int iterableLen = 0;
    if (iterable->getType() == TYPE_ARRAY) {
        iterableLen = (int)std::static_pointer_cast<ArrayObject>(iterable)->getValue()->size();
    } else if (iterable->getType() == TYPE_STRING) {
        iterableLen = (int)std::static_pointer_cast<StringObject>(iterable)->getValue().size();
    } else {
        Error::runtimeError("Invalid type for iterable (expected array or string, got " + typeStrings[iterable->getType()] + ")",
                            node->getIterable()->getLineNumber(), node->getIterable()->getColumnNumber());
    }

    auto *forLoopScope = new SymbolTable(arg, true, arg->isFunction());
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
        iteratorSymbol->setValue(iterable->subscript(std::make_shared<IntObject>(i)));
        node->getBody()->evaluate(this, forLoopScope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IfStatementNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> condition = node->getCondition()->evaluate(this, arg);
    if (condition->getType() != TYPE_BOOL) {
        Error::runtimeError("Invalid type for if condition (expected bool, got " + typeStrings[condition->getType()] + ")",
                            node->getCondition()->getLineNumber(), node->getCondition()->getColumnNumber());
    } else if (std::static_pointer_cast<BoolObject>(condition)->getValue()) {
        node->getIfBody()->evaluate(this, arg);
        return nullptr;
    } else if (node->getElseBody() != nullptr) {
        node->getElseBody()->evaluate(this, arg);
        return nullptr;
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(RepeatStatementNode *node, SymbolTable* arg) {
    auto *repeatLoopScope = new SymbolTable(arg, true, arg->isFunction());
    std::shared_ptr<Object> count = node->getCount()->evaluate(this, arg);
    if (count->getType() != TYPE_INT) {
        Error::runtimeError("Invalid type for repeat count (expected int, got " + typeStrings[count->getType()] + ")",
                            node->getCount()->getLineNumber(), node->getCount()->getColumnNumber());
    }

    for (int i = 0; i < std::static_pointer_cast<IntObject>(count)->getValue(); i++) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->getBody()->evaluate(this, repeatLoopScope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(WhileStatementNode *node, SymbolTable* arg) {
    auto *whileLoopScope = new SymbolTable(arg, true, arg->isFunction());
    std::shared_ptr<Object> condition = node->getCondition()->evaluate(this, arg);
    if (condition->getType() != TYPE_BOOL) {
        Error::runtimeError("Invalid type for while condition (expected bool, got " + typeStrings[condition->getType()] + ")",
                            node->getCondition()->getLineNumber(), node->getCondition()->getColumnNumber());
    }

    while (std::static_pointer_cast<BoolObject>(condition)->getValue()) {
        if (backtracking || returning) {
            backtracking = false;
            if (breaking) {
                breaking = false;
                break;
            }
        }

        node->getBody()->evaluate(this, whileLoopScope);
        condition = node->getCondition()->evaluate(this, arg);

        if (condition->getType() != TYPE_BOOL) {
            Error::runtimeError("Invalid type for while condition (expected bool, got " + typeStrings[condition->getType()] + ")",
                                node->getCondition()->getLineNumber(), node->getCondition()->getColumnNumber());
        }
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionDeclarationNode *node, SymbolTable* arg) {
    std::shared_ptr<Object> functionObject = std::make_shared<FunctionObject>(node->getBody(), *node->getParameters());
    Symbol functionSymbol(node->getIdentifier(), functionObject);
    arg->insert(functionSymbol);

    auto *functionScope = new SymbolTable(arg, arg->isLoop(), true);
    for (auto &param : *node->getParameters()) {
        functionScope->insert(Symbol(param));
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(FunctionStatementNode *node, SymbolTable* arg) {
    std::string name = node->getIdentifier();
    Symbol *functionSymbol = arg->lookup(name, false);
    std::shared_ptr<FunctionObject> functionObject = std::static_pointer_cast<FunctionObject>(functionSymbol->getValue());

    if (functionObject->isBuiltin()) {
        std::vector<std::shared_ptr<Object>> arguments;
        for (auto &argument : *node->getArguments()) {
            arguments.push_back(argument->evaluate(this, arg));
        }
        return BuiltinFunctions::handleBuiltinFunction(name, &arguments, node->getLineNumber(), node->getColumnNumber());
    }

    returnVal = std::make_shared<VoidObject>();

    auto *functionScope = new SymbolTable(arg->getGlobalScope(), false, true);
    for (int i = 0; i < (int)node->getArguments()->size(); i++) {
        std::string argName = functionObject->getParameters()->at(i);
        std::shared_ptr<Object> argValue = node->getArguments()->at(i)->evaluate(this, arg);
        functionScope->insert(Symbol(argName, argValue));
    }

    functionObject->getBody()->evaluate(this, functionScope);
    returning = false;
    return returnVal;
}

std::shared_ptr<Object> InterpreterVisitor::visit(CompoundStatementNode *node, SymbolTable* arg) {
    auto *scope = new SymbolTable(arg, arg->isLoop(), arg->isFunction());
    for (auto &statement : *node->getStatements()) {
        if (backtracking) return nullptr;
        statement->evaluate(this, scope);
    }

    return nullptr;
}

std::shared_ptr<Object> InterpreterVisitor::visit(IdentifierNode *node, SymbolTable* arg) {
    std::string name = node->getName();
    return arg->lookup(name, false)->getValue();
}

std::shared_ptr<Object> InterpreterVisitor::visit(LiteralNode *node, SymbolTable* arg) {
    switch (node->getType()) {
        case TYPE_INT:
            return std::make_shared<IntObject>(std::stoi(node->getValue()));
        case TYPE_FLOAT:
            return std::make_shared<FloatObject>(std::stof(node->getValue()));
        case TYPE_BOOL:
            return std::make_shared<BoolObject>(node->getValue() == "true");
        case TYPE_STRING:
            return std::make_shared<StringObject>(node->getValue().substr(1, node->getValue().length() - 2));
        default:
            return nullptr;
    }
}
