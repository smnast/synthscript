#include "Visitor/SemanticAnalysisVisitor.h"
#include "AST/ASTNodes.h"
#include "Object/FunctionObject.h"

void SemanticAnalysisVisitor::visit(ProgramNode *node, SymbolTable* arg) {
    auto *globalTable = new SymbolTable(nullptr, false, false);
    BuiltinFunctions::registerBuiltinFunctions(globalTable);

    for (auto &statement : *node->getStatements()) {
        statement->analyze(this, globalTable);
    }
    delete globalTable;
}

void SemanticAnalysisVisitor::visit(BinOpNode *node, SymbolTable* arg) {
    node->getLeftNode()->analyze(this, arg);
    node->getRightNode()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(CastOpNode *node, SymbolTable* arg) {
    node->getOperand()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(SubscriptOpNode *node, SymbolTable* arg) {
    node->getIdentifier()->analyze(this, arg);
    node->getIndex()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(UnaryOpNode *node, SymbolTable* arg) {
    node->getOperand()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(ArrayLiteralNode *node, SymbolTable* arg) {
    for (auto &element : *node->getValues()) {
        element->analyze(this, arg);
    }
}

std::string SemanticAnalysisVisitor::getArrayIdentifier(SubscriptOpNode *node, SymbolTable* arg) {
    auto subscriptOpNode = dynamic_cast<SubscriptOpNode*>(node->getIdentifier());
    if (subscriptOpNode != nullptr) {
        return getArrayIdentifier(subscriptOpNode, arg);
    } else {
        return dynamic_cast<IdentifierNode*>(node->getIdentifier())->getName();
    }
}

void SemanticAnalysisVisitor::visit(AssignmentNode *node, SymbolTable* arg) {
    if (dynamic_cast<SubscriptOpNode*>(node->getIdentifier()) != nullptr) {
        auto *subscriptOpNode = dynamic_cast<SubscriptOpNode*>(node->getIdentifier());
        std::string name = getArrayIdentifier(subscriptOpNode, arg);
        if (!arg->contains(name, false)) {
            semanticError("Undeclared identifier '" + name + "'", node->getLineNumber(), node->getColumnNumber());
        }
    } else if (dynamic_cast<IdentifierNode*>(node->getIdentifier()) != nullptr) {
        std::string name = dynamic_cast<IdentifierNode*>(node->getIdentifier())->getName();
        if (!arg->contains(name, false)) {
            arg->insert(Symbol(name));
        }
    } else {
        semanticError("Invalid assignment operand", node->getLineNumber(), node->getColumnNumber());
    }
    node->getIdentifier()->analyze(this, arg);
    node->getValue()->analyze(this, arg);
}

void SemanticAnalysisVisitor::visit(BreakStatementNode *node, SymbolTable* arg) {
    if (!arg->isLoop()) {
        semanticError("Break statement outside of loop", node->getLineNumber(), node->getColumnNumber());
    }
}

void SemanticAnalysisVisitor::visit(ContinueStatementNode *node, SymbolTable* arg) {
    if (!arg->isLoop()) {
        semanticError("Continue statement outside of loop", node->getLineNumber(), node->getColumnNumber());
    }
}

void SemanticAnalysisVisitor::visit(ReturnStatementNode *node, SymbolTable* arg) {
    if (!arg->isFunction()) {
        semanticError("Return statement outside of function", node->getLineNumber(), node->getColumnNumber());
    }
    if (node->getValue() != nullptr) {
        node->getValue()->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(ForStatementNode *node, SymbolTable* arg) {
    std::string identifier = node->getIdentifier();
    node->getStart()->analyze(this, arg);
    node->getEnd()->analyze(this, arg);

    auto *forLoopScope = new SymbolTable(arg, true, arg->isFunction());
    forLoopScope->insert(Symbol(identifier));
    node->getBody()->analyze(this, forLoopScope);
}

void SemanticAnalysisVisitor::visit(IfStatementNode *node, SymbolTable* arg) {
    node->getCondition()->analyze(this, arg);

    node->getIfBody()->analyze(this, arg);
    if (node->getElseBody() != nullptr) {
        node->getElseBody()->analyze(this, arg);
    }
}

void SemanticAnalysisVisitor::visit(WhileStatementNode *node, SymbolTable* arg) {
    auto *whileLoopScope = new SymbolTable(arg, true, arg->isFunction());
    node->getCondition()->analyze(this, whileLoopScope);
    node->getBody()->analyze(this, whileLoopScope);
}

void SemanticAnalysisVisitor::visit(FunctionDeclarationNode *node, SymbolTable* arg) {
    if (!arg->isGlobalScope()) {
        semanticError("Function declaration outside of global scope", node->getLineNumber(), node->getColumnNumber());
    }

    if (arg->contains(node->getIdentifier(), false)) {
        semanticError("Identifier '" + node->getIdentifier() + "' already declared", node->getLineNumber(), node->getColumnNumber());
    }

    std::shared_ptr<Object> functionObject = std::make_shared<FunctionObject>(node->getBody(), *node->getParameters());
    Symbol functionSymbol(node->getIdentifier(), functionObject);
    arg->insert(functionSymbol);

    auto *functionScope = new SymbolTable(arg, arg->isLoop(), true);
    for (auto &param : *node->getParameters()) {
        functionScope->insert(Symbol(param));
    }

    node->getBody()->analyze(this, functionScope);
}

void SemanticAnalysisVisitor::visit(FunctionStatementNode *node, SymbolTable* arg) {
    std::string name = node->getIdentifier();

    if (!arg->contains(name, false)) {
        semanticError("Undeclared identifier '" + name + "'", node->getLineNumber(), node->getColumnNumber());
    }
    for (auto &param : *node->getArguments()) {
        param->analyze(this, arg);
    }

    Symbol *functionSymbol = arg->lookup(name, false);
    if (functionSymbol->getType() != TYPE_FUNCTION) {
        semanticError("Identifier '" + name + "' is not a function", node->getLineNumber(), node->getColumnNumber());
    } else {
        FunctionObject *functionObject = dynamic_cast<FunctionObject*>(functionSymbol->getValue().get());
        if (functionObject->getParameters()->size() != node->getArguments()->size()) {
            semanticError("Incorrect number of arguments to function '" + name + "'", node->getLineNumber(), node->getColumnNumber());
        }
    }
}

void SemanticAnalysisVisitor::visit(CompoundStatementNode *node, SymbolTable* arg) {
    auto *scope = new SymbolTable(arg, arg->isLoop(), arg->isFunction());
    for (auto &statement : *node->getStatements()) {
        statement->analyze(this, scope);
    }
}

void SemanticAnalysisVisitor::visit(IdentifierNode *node, SymbolTable* arg) {
    std::string name = node->getName();
    if (!arg->contains(name, false)) {
        semanticError("Undeclared identifier '" + name + "'", node->getLineNumber(), node->getColumnNumber());
    } else if (arg->lookup(name, false)->getType() == TYPE_FUNCTION) {
        semanticError("Identifier '" + name + "' is a function", node->getLineNumber(), node->getColumnNumber());
    }
}

void SemanticAnalysisVisitor::visit(LiteralNode *node, SymbolTable* arg) {
}

void SemanticAnalysisVisitor::semanticError(const std::string &message, int line, int column) {
    Error::posError(message, line, column, true);
}
