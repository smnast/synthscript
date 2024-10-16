#ifndef SYNTHSCRIPT_SYMBOLTABLE_H
#define SYNTHSCRIPT_SYMBOLTABLE_H

#include "symbol.h"
#include <unordered_map>

class SymbolTable {
public:
    SymbolTable(SymbolTable *enclosing_scope, bool loop, bool function);

    ~SymbolTable() {
        for (auto &child : child_scope) {
            delete child;
        }
    }

    void insert(Symbol symbol);
    bool contains(const std::string &name, bool current_scope);
    Symbol *lookup(const std::string &name, bool current_scope);

    bool is_loop() const;
    bool is_function() const;
    bool is_global_scope() const;

    SymbolTable *get_global_scope() const;

protected:
    void add_child(SymbolTable *symbol_table);

private:
    std::unordered_map<std::string, Symbol> symbols;
    SymbolTable *enclosing_scope;
    SymbolTable *global_scope;
    std::vector<SymbolTable *> child_scope;
    bool loop = false;
    bool function = false;
};

#endif // SYNTHSCRIPT_SYMBOLTABLE_H
