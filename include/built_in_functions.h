#ifndef SYNTHSCRIPT_BUILTINFUNCTIONS_H
#define SYNTHSCRIPT_BUILTINFUNCTIONS_H

#include "symbol/symbol_table.h"
#include <functional>

struct BuiltinFunction {
    std::function<std::shared_ptr<Object>(std::vector<std::shared_ptr<Object>>, int, int)> function;
    int paramCount;
};

class BuiltinFunctions {
public:
    static void registerBuiltinFunctions(SymbolTable *symbolTable);
    static std::shared_ptr<Object> handleBuiltinFunction(const std::string& identifier, std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_output(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_input(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_read(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_write(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_append(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_currentDirectory(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_len(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_sum(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_product(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
private:
    static std::unordered_map<std::string, BuiltinFunction> builtinFunctions;
};

#endif //SYNTHSCRIPT_BUILTINFUNCTIONS_H
