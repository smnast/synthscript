#ifndef SYNTHSCRIPT_BUILTINFUNCTIONS_H
#define SYNTHSCRIPT_BUILTINFUNCTIONS_H

#include "symbol/symbol_table.h"
#include <functional>

struct BuiltinFunction {
    std::function<std::shared_ptr<Object>(std::vector<std::shared_ptr<Object>>, int, int)> function;
    int param_count;
};

class BuiltinFunctions {
public:
    static void register_built_in_functions(SymbolTable *symbol_table);
    static std::shared_ptr<Object> handle_built_in_function(const std::string &identifier, std::vector<std::shared_ptr<Object>> *arguments, int line, int col);

    static std::shared_ptr<Object> built_in_output(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_input(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_read(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_write(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_append(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_current_directory(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_len(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_sum(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> built_in_product(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);

private:
    static std::unordered_map<std::string, BuiltinFunction> built_in_functions;
};

#endif // SYNTHSCRIPT_BUILTINFUNCTIONS_H
