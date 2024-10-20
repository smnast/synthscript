#ifndef SYNTHSCRIPT_SYMBOL_H
#define SYNTHSCRIPT_SYMBOL_H

#include "object/object.h"

/**
 * @class Symbol
 * @brief Represents a symbol with a name and value.
 *
 * @note
 * The type of the symbol is determined by the type of the associated value.
 */
class Symbol {
public:
    /**
     * @brief Constructs a symbol with no name and a null value.
     */
    Symbol() = default;

    /**
     * @brief Constructs a symbol with the given name and a null value.
     *
     * @param name The name of the symbol.
     */
    explicit Symbol(std::string name);

    /**
     * @brief Constructs a symbol with the given name and value.
     *
     * @param name The name of the symbol.
     * @param value The value of the symbol.
     */
    Symbol(std::string name, std::shared_ptr<Object> value);

    /**
     * @brief Get the name of the symbol.
     * @return The name of the symbol.
     */
    std::string get_name() const;

    /**
     * @brief Set the value of the symbol.
     * @param value The new value of the symbol.
     */
    void set_value(std::shared_ptr<Object> value);

    /**
     * @brief Get the value of the symbol.
     * @return The value of the symbol.
     */
    std::shared_ptr<Object> get_value() const;

    /**
     * @brief Get the type of the symbol.
     * @return The type of the symbol.
     * 
     * @note
     * If the value of the symbol is null, the type of the symbol is undefined.
     */
    Type get_type() const;

private:
    /**
     * @brief The name of the symbol.
     */
    std::string name;

    /**
     * @brief The value of the symbol.
     */
    std::shared_ptr<Object> value;
};

#endif // SYNTHSCRIPT_SYMBOL_H
