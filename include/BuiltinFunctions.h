#ifndef SYNTHSCRIPT_BUILTINFUNCTIONS_H
#define SYNTHSCRIPT_BUILTINFUNCTIONS_H

#include <functional>
#include "Symbol/SymbolTable.h"

struct BuiltinFunction {
    std::function<std::shared_ptr<Object>(std::vector<std::shared_ptr<Object>>, int, int)> function;
    int paramCount;
};

class BuiltinFunctions {
public:
    static void registerBuiltinFunctions(SymbolTable *symbolTable);
    static std::shared_ptr<Object> handleBuiltinFunction(const std::string& identifier, std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
    static std::shared_ptr<Object> builtin_output(std::vector<std::shared_ptr<Object>> *arguments, int line, int col);
private:
    static std::unordered_map<std::string, BuiltinFunction> builtinFunctions;
};

#endif //SYNTHSCRIPT_BUILTINFUNCTIONS_H
