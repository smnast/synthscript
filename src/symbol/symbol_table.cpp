#include "symbol/symbol_table.h"

SymbolTable::SymbolTable(SymbolTable *enclosingScope, bool loop, bool function) {
    this->enclosingScope = enclosingScope;
    this->loop = loop;
    this->function = function;
    globalScope = enclosingScope == nullptr ? this : enclosingScope->get_global_scope();
    if (enclosingScope != nullptr) {
        enclosingScope->add_child(this);
    }
}

void SymbolTable::insert(Symbol symbol) {
    symbols[symbol.get_name()] = symbol;
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

bool SymbolTable::is_loop() const {
    return loop;
}

bool SymbolTable::is_function() const {
    return function;
}

bool SymbolTable::is_global_scope() const {
    return this == globalScope;
}

SymbolTable *SymbolTable::get_global_scope() const {
    return globalScope;
}

void SymbolTable::add_child(SymbolTable *symbolTable) {
    childScope.push_back(symbolTable);
}
