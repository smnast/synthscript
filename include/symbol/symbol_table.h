#ifndef SYNTHSCRIPT_SYMBOLTABLE_H
#define SYNTHSCRIPT_SYMBOLTABLE_H

#include "symbol.h"
#include <unordered_map>

class SymbolTable {
public:
    SymbolTable(SymbolTable *enclosingScope, bool loop, bool function);
    ~SymbolTable() {
        for (auto &child : childScope) {
            delete child;
        }
    }

    void insert(Symbol symbol);
    bool contains(const std::string &name, bool currentScope);
    Symbol *lookup(const std::string &name, bool currentScope);
    bool is_loop() const;
    bool is_function() const;
    bool is_global_scope() const;
    SymbolTable *get_global_scope() const;

protected:
    void add_child(SymbolTable *symbolTable);

private:
    std::unordered_map<std::string, Symbol> symbols;
    SymbolTable *enclosingScope;
    SymbolTable *globalScope;
    std::vector<SymbolTable*> childScope;
    bool loop = false;
    bool function = false;
};

#endif //SYNTHSCRIPT_SYMBOLTABLE_H
