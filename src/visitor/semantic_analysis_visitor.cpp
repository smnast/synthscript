#include "visitor/semantic_analysis_visitor.h"
#include "AST/AST_nodes.h"
#include "object/function_object.h"

void SemanticAnalysisVisitor::visit(ProgramNode *node, SymbolTable* arg) {
    auto *globalTable = new SymbolTable(nullptr, false, false);
    BuiltinFunctions::registerBuiltinFunctions(globalTable);

    for (auto &statement : *node->get_statements()) {
        statement->analyze(this, globalTable);
    }
    delete globalTable;
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

std::string SemanticAnalysisVisitor::getArrayIdentifier(SubscriptOpNode *node, SymbolTable* arg) {
    auto subscriptOpNode = dynamic_cast<SubscriptOpNode*>(node->get_identifier());
    if (subscriptOpNode != nullptr) {
        return getArrayIdentifier(subscriptOpNode, arg);
    } else {
        return dynamic_cast<IdentifierNode*>(node->get_identifier())->get_name();
    }
}

void SemanticAnalysisVisitor::visit(AssignmentNode *node, SymbolTable* arg) {
    if (dynamic_cast<SubscriptOpNode*>(node->get_identifier()) != nullptr) {
        auto *subscriptOpNode = dynamic_cast<SubscriptOpNode*>(node->get_identifier());
        std::string name = getArrayIdentifier(subscriptOpNode, arg);
        if (!arg->contains(name, false)) {
            semanticError("Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
        }
    } else if (dynamic_cast<IdentifierNode*>(node->get_identifier()) != nullptr) {
        std::string name = dynamic_cast<IdentifierNode*>(node->get_identifier())->get_name();
        if (!arg->contains(name, false)) {
            arg->insert(Symbol(name));
        }
    } else {
        semanticError("Invalid assignment operand", node->get_line(), node->get_column());
    }
    node->get_identifier()->analyze(this, arg);
    node->get_value()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(BreakStatementNode *node, SymbolTable* arg) {
    if (!arg->is_loop()) {
        semanticError("Break statement outside of loop", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(ContinueStatementNode *node, SymbolTable* arg) {
    if (!arg->is_loop()) {
        semanticError("Continue statement outside of loop", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(ReturnStatementNode *node, SymbolTable* arg) {
    if (!arg->is_function()) {
        semanticError("Return statement outside of function", node->get_line(), node->get_column());
    }
    if (node->get_value() != nullptr) {
        node->get_value()->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(ForStatementNode *node, SymbolTable* arg) {
    std::string identifier = node->get_identifier();
    node->get_iterable()->analyze(this, arg);

    auto *forLoopScope = new SymbolTable(arg, true, arg->is_function());
    forLoopScope->insert(Symbol(identifier));
    node->get_body()->analyze(this, forLoopScope);
}

void SemanticAnalysisVisitor::visit(IfStatementNode *node, SymbolTable* arg) {
    node->get_condition()->analyze(this, arg);

    node->get_if_body()->analyze(this, arg);
    if (node->get_else_body() != nullptr) {
        node->get_else_body()->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(RepeatStatementNode *node, SymbolTable* arg) {
    auto *repeatLoopScope = new SymbolTable(arg, true, arg->is_function());
    node->get_count()->analyze(this, repeatLoopScope);
    node->get_body()->analyze(this, repeatLoopScope);
}

void SemanticAnalysisVisitor::visit(WhileStatementNode *node, SymbolTable* arg) {
    auto *whileLoopScope = new SymbolTable(arg, true, arg->is_function());
    node->get_condition()->analyze(this, whileLoopScope);
    node->get_body()->analyze(this, whileLoopScope);
}

void SemanticAnalysisVisitor::visit(FunctionDeclarationNode *node, SymbolTable* arg) {
    if (!arg->is_global_scope()) {
        semanticError("Function declaration outside of global scope", node->get_line(), node->get_column());
    }

    if (arg->contains(node->get_identifier(), false)) {
        semanticError("Identifier '" + node->get_identifier() + "' already declared", node->get_line(), node->get_column());
    }

    std::shared_ptr<Object> functionObject = std::make_shared<FunctionObject>(node->get_body(), *node->get_parameters());
    Symbol functionSymbol(node->get_identifier(), functionObject);
    arg->insert(functionSymbol);

    auto *functionScope = new SymbolTable(arg, arg->is_loop(), true);
    for (auto &param : *node->get_parameters()) {
        functionScope->insert(Symbol(param));
    }

    node->get_body()->analyze(this, functionScope);
}

void SemanticAnalysisVisitor::visit(FunctionStatementNode *node, SymbolTable* arg) {
    std::string name = node->get_identifier();

    if (!arg->contains(name, false)) {
        semanticError("Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
    }
    for (auto &param : *node->get_arguments()) {
        param->analyze(this, arg);
    }

    Symbol *functionSymbol = arg->lookup(name, false);
    if (functionSymbol->get_type() != TYPE_FUNCTION) {
        semanticError("Identifier '" + name + "' is not a function", node->get_line(), node->get_column());
    } else {
        FunctionObject *functionObject = dynamic_cast<FunctionObject*>(functionSymbol->get_value().get());
        if (functionObject->get_parameters()->size() != node->get_arguments()->size()) {
            semanticError("Incorrect number of arguments to function '" + name + "'", node->get_line(), node->get_column());
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
        semanticError("Undeclared identifier '" + name + "'", node->get_line(), node->get_column());
    } else if (arg->lookup(name, false)->get_type() == TYPE_FUNCTION) {
        semanticError("Identifier '" + name + "' is a function", node->get_line(), node->get_column());
    }
}

void SemanticAnalysisVisitor::visit(LiteralNode *node, SymbolTable* arg) {
}

void SemanticAnalysisVisitor::semanticError(const std::string &message, int line, int column) {
    Error::error_at_pos(message, line, column, true);
}
