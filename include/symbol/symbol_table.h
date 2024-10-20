#ifndef SYNTHSCRIPT_SYMBOLTABLE_H
#define SYNTHSCRIPT_SYMBOLTABLE_H

#include "symbol.h"
#include <unordered_map>

/**
 * @class SymbolTable
 * @brief A class representing a symbol table that manages symbols within different scopes.
 *
 * The SymbolTable class manages symbols within different scopes, such as global, function, and loop
 * scopes. It supports inserting symbols, checking for their existence, and retrieving them.
 *
 * @note
 * The SymbolTable class assumes ownership of child symbol tables and is responsible for their
 * deletion.
 */
class SymbolTable {
public:
    /**
     * @brief Constructs a new SymbolTable.
     * @param enclosing_scope A pointer to the enclosing scope's SymbolTable.
     * @param loop A boolean indicating if the symbol table is within a loop.
     * @param function A boolean indicating if the symbol table is within a function.
     * 
     * @note
     * If enclosing_scope is nullptr, the SymbolTable is considered the global scope.
     */
    SymbolTable(SymbolTable *enclosing_scope, bool loop = false, bool function = false);

    ~SymbolTable();

    /**
     * @brief Insert a symbol into the symbol table.
     * @param symbol The symbol to insert.
     */
    void insert(Symbol symbol);

    /**
     * @brief Check if the symbol table contains a symbol with the given name.
     *
     * If current_scope is true, only the current scope is checked. Otherwise, the enclosing scopes
     * are checked as well.
     *
     * @param name The name of the symbol to check for.
     * @param current_scope Whether to only check the current scope.
     * @return True if the symbol table contains a symbol with the given name, false otherwise.
     */
    bool contains(const std::string &name, bool current_scope);

    /**
     * @brief Get the symbol from the symbol table with the given name.
     *
     * If current_scope is true, only the current scope is checked. Otherwise, the enclosing scopes
     * are checked as well.
     *
     * @param name The name of the symbol to get.
     * @param current_scope Whether to only check the current scope.
     * @return The symbol with the given name, or nullptr if the symbol does not exist.
     */
    Symbol *get(const std::string &name, bool current_scope);

    /**
     * @brief Check if the symbol table is a loop scope.
     * @return True if the symbol table is a loop scope, false otherwise.
     */
    bool is_loop() const;

    /**
     * @brief Check if the symbol table is a function scope.
     * @return True if the symbol table is a function scope, false otherwise.
     */
    bool is_function() const;

    /**
     * @brief Check if the symbol table is the global scope.
     * @return True if the symbol table is the global scope, false otherwise.
     */
    bool is_global_scope() const;

    /**
     * @brief Get the symbol table with the global scope.
     * @return The global scope symbol table.
     */
    SymbolTable *get_global_scope() const;

protected:
    /**
     * @brief Add a symbol table as a child.
     * @param symbol_table
     */
    void add_child(SymbolTable *symbol_table);

private:
    /**
     * @brief The symbols in the symbol table.
     */
    std::unordered_map<std::string, Symbol> symbols;

    /**
     * @brief The enclosing scope of the symbol table, if it exists.
     *
     * @note
     * The enclosing scope is not deleted by the destructor. 
     */
    SymbolTable *enclosing_scope;

    /**
     * @brief The symbol table with the global scope.
     *
     * @note
     * The global scope is not deleted by the destructor.
     */
    SymbolTable *global_scope;

    /**
     * @brief The child scopes of the symbol table.
     * 
     * @note
     * The child scopes are deleted by the destructor.
     */
    std::vector<SymbolTable *> child_scopes;

    /**
     * @brief Whether the symbol table is a loop scope.
     */
    bool loop = false;

    /**
     * @brief Whether the symbol table is a function scope.
     */
    bool function = false;
};

#endif // SYNTHSCRIPT_SYMBOLTABLE_H