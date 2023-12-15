#ifndef SYNTHSCRIPT_SYMBOLTABLE_H
#define SYNTHSCRIPT_SYMBOLTABLE_H

#include <unordered_map>

#include "Symbol.h"

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
    bool isLoop() const;
    bool isFunction() const;
    bool isGlobalScope() const;
    SymbolTable *getGlobalScope() const;
protected:
    void addChild(SymbolTable *symbolTable);
private:
    std::unordered_map<std::string, Symbol> symbols;
    SymbolTable *enclosingScope, *globalScope;
    std::vector<SymbolTable*> childScope;
    bool loop = false, function = false;
};

#endif //SYNTHSCRIPT_SYMBOLTABLE_H
