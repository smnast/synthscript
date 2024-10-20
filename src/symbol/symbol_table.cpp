#include "symbol/symbol_table.h"

SymbolTable::SymbolTable(SymbolTable *enclosing_scope, bool loop, bool function) {
    this->enclosing_scope = enclosing_scope;
    this->loop = loop;
    this->function = function;

    if (enclosing_scope) {
        global_scope = enclosing_scope->get_global_scope();
        enclosing_scope->add_child(this);
    } else {
        // If there is no enclosing scope, this is the global scope
        global_scope = this;
    }
}

SymbolTable::~SymbolTable() {
    for (auto *child_scope : child_scopes) {
        delete child_scope;
    }
}

void SymbolTable::insert(Symbol symbol) {
    symbols[symbol.get_name()] = symbol;
}

bool SymbolTable::contains(const std::string &name, bool current_scope) {
    // Search this scope for the symbol
    if (symbols.find(name) != symbols.end()) {
        return true;
    }
    // Search the parent scope for the symbol
    else if (!current_scope && enclosing_scope) {
        return enclosing_scope->contains(name, false);
    } else {
        return false;
    }
}

Symbol *SymbolTable::get(const std::string &name, bool current_scope) {
    // Search this scope for the symbol
    if (contains(name, true)) {
        return &symbols[name];
    }
    // Search the parent scope for the symbol
    else if (!current_scope && enclosing_scope) {
        return enclosing_scope->get(name, false);
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
    child_scopes.push_back(symbol_table);
}
