#include "symbol/symbol_table.h"

SymbolTable::SymbolTable(SymbolTable *enclosing_scope, bool loop, bool function) {
    this->enclosing_scope = enclosing_scope;
    this->loop = loop;
    this->function = function;
    global_scope = enclosing_scope == nullptr ? this : enclosing_scope->get_global_scope();
    if (enclosing_scope != nullptr) {
        enclosing_scope->add_child(this);
    }
}

void SymbolTable::insert(Symbol symbol) {
    symbols[symbol.get_name()] = symbol;
}

bool SymbolTable::contains(const std::string &name, bool current_scope) {
    if (symbols.find(name) != symbols.end()) {
        return true;
    } else {
        return (!current_scope && enclosing_scope != nullptr && enclosing_scope->contains(name, false));
    }
}

Symbol *SymbolTable::lookup(const std::string &name, bool current_scope) {
    if (contains(name, true)) {
        return &symbols[name];
    } else if (!current_scope && enclosing_scope != nullptr) {
        return enclosing_scope->lookup(name, false);
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
    return this == global_scope;
}

SymbolTable *SymbolTable::get_global_scope() const {
    return global_scope;
}

void SymbolTable::add_child(SymbolTable *symbol_table) {
    child_scope.push_back(symbol_table);
}
