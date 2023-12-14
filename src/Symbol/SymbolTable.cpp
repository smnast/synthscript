#include "Symbol/SymbolTable.h"

SymbolTable::SymbolTable(SymbolTable *enclosingScope, bool loop, bool function) {
    this->enclosingScope = enclosingScope;
    this->loop = loop;
    this->function = function;
    globalScope = enclosingScope == nullptr ? this : enclosingScope->getGlobalScope();
    if (enclosingScope != nullptr) {
        this->functionReturnType = enclosingScope->getReturnType();
        enclosingScope->addChild(this);
    }
}

void SymbolTable::insert(Symbol symbol) {
    symbols[symbol.getName()] = symbol;
}

bool SymbolTable::contains(const std::string &name, bool currentScope) {
    if (symbols.find(name) != symbols.end()) return true;
    else return (!currentScope && enclosingScope != nullptr && enclosingScope->contains(name, false));
}

Symbol *SymbolTable::lookup(const std::string &name, bool currentScope) {
    if (contains(name, true)) {
        return &symbols[name];
    } else if (!currentScope && enclosingScope != nullptr) {
        return enclosingScope->lookup(name, false);
    } else {
        return nullptr;
    }
}

bool SymbolTable::isLoop() const {
    return loop;
}

bool SymbolTable::isFunction() const {
    return function;
}

bool SymbolTable::isGlobalScope() const {
    return enclosingScope == globalScope;
}

void SymbolTable::setReturnType(Type type) {
    functionReturnType = type;
}

Type SymbolTable::getReturnType() const {
    return functionReturnType;
}

SymbolTable *SymbolTable::getGlobalScope() const {
    return globalScope;
}

void SymbolTable::addChild(SymbolTable *symbolTable) {
    childScope.push_back(symbolTable);
}
