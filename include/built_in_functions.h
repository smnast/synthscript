#ifndef SYNTHSCRIPT_BUILTINFUNCTIONS_H
#define SYNTHSCRIPT_BUILTINFUNCTIONS_H

#include "error_manager.h"
#include "symbol/symbol_table.h"
#include <functional>

struct BuiltInFunction;

/**
 * @class BuiltInFunctions
 * @brief A class representing the built-in functions that can be called in SynthScript.
 */
class BuiltInFunctions {
public:
    /**
     * @brief Create a new BuiltInFunctions object.
     * @param error_manager The error manager to use for error handling.
     *
     * @note
     * The BuiltInFunctions class does not assume ownership of the error manager.
     */
    BuiltInFunctions(ErrorManager *error_manager);

    /**
     * @brief Register the built-in functions with the given symbol table.
     * @param symbol_table The symbol table to register the built-in functions with.
     */
    void register_built_in_functions(SymbolTable *symbol_table);

    /**
     * @brief Handle a built-in function call.
     * @param identifier The identifier of the built-in function.
     * @param arguments The arguments to the function.
     * @param line The line number where the function was called.
     * @param col The column number where the function was called.
     * @return The result of the function.
     */
    std::shared_ptr<Object>
    handle_built_in_function(const std::string &identifier,
                             std::vector<std::shared_ptr<Object>> *arguments,
                             int line,
                             int col);

    std::shared_ptr<Object>
    built_in_output(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_input(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_read(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_write(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_append(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_current_directory(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_len(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_sum(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    std::shared_ptr<Object>
    built_in_product(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);

private:
    /**
     * @brief Maps identifiers to their built-in function.
     */
    std::unordered_map<std::string, BuiltInFunction> built_in_functions;

    /**
     * @brief The error manager to use for error handling.
     */
    ErrorManager *error_manager;
};

/**
 * @struct BuiltInFunction
 * @brief Represents a built-in function in the SynthScript language.
 */
struct BuiltInFunction {
    /**
     * @brief The function that the built-in function calls.
     * @param arguments The arguments to the function.
     * @param line The line number where the function was called.
     * @param col The column number where the function was called.
     * @return The result of the function.
     */
    std::function<std::shared_ptr<Object>(std::vector<std::shared_ptr<Object>>, int, int)> function;

    /**
     * @brief The number of parameters the function takes.
     */
    int param_count;
};

#define BUILT_IN_FUNCTION(name, param_count, instance)                                             \
    {                                                                                              \
        #name, {                                                                                   \
            [instance](std::vector<std::shared_ptr<Object>> arguments,                             \
                       int line,                                                                   \
                       int col) -> std::shared_ptr<Object> {                                       \
                return instance->built_in_##name(&arguments, line, col);                           \
            },                                                                                     \
                param_count                                                                        \
        }                                                                                          \
    }

#endif // SYNTHSCRIPT_BUILTINFUNCTIONS_H
