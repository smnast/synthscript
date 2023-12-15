#include "BuiltinFunctions.h"
#include "Error.h"
#include "Object/StringObject.h"
#include "Object/VoidObject.h"
#include "Object/FunctionObject.h"

std::unordered_map<std::string, BuiltinFunction> BuiltinFunctions::builtinFunctions = {
        {"output", {[](std::vector<std::shared_ptr<Object>> arguments, int line, int col) -> std::shared_ptr<Object> {
            return BuiltinFunctions::builtin_output(&arguments, line, col);
        }, 1}}
};

void BuiltinFunctions::registerBuiltinFunctions(SymbolTable *symbolTable) {
    for (const auto &builtinFunction : builtinFunctions) {
        std::vector<std::string> parameters(builtinFunction.second.paramCount);
        std::shared_ptr<Object> functionObject = std::make_shared<FunctionObject>(nullptr, parameters, true);
        Symbol functionSymbol(builtinFunction.first, functionObject);
        symbolTable->insert(functionSymbol);
    }
}

std::shared_ptr<Object> BuiltinFunctions::handleBuiltinFunction(const std::string &identifier, std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    return builtinFunctions[identifier].function(*arguments, line, col);
}

std::shared_ptr<Object> BuiltinFunctions::builtin_output(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> castObj = arguments->at(0)->cast(TYPE_STRING);
    if (castObj == nullptr) {
        Error::runtimeError("Invalid argument to builtin output of type " + typeStrings[arguments->at(0)->getType()], line, col);
    } else {
        printf("%s\n", std::static_pointer_cast<StringObject>(castObj)->getValue().c_str());
    }

    return std::make_shared<VoidObject>();
}
