#include "built_in_functions.h"
#include "error.h"
#include "object/string_object.h"
#include "object/void_object.h"
#include "object/function_object.h"
#include "object/array_object.h"
#include "object/int_object.h"
#include <csignal>

#define BUILTIN_FUNCTION(name, paramCount) {#name, {[](std::vector<std::shared_ptr<Object>> arguments, int line, int col) -> std::shared_ptr<Object> { \
    return BuiltinFunctions::builtin_##name(&arguments, line, col); \
}, paramCount}}

std::unordered_map<std::string, BuiltinFunction> BuiltinFunctions::builtinFunctions = {
        BUILTIN_FUNCTION(output, 1),
        BUILTIN_FUNCTION(input, 0),
        BUILTIN_FUNCTION(read, 1),
        BUILTIN_FUNCTION(write, 2),
        BUILTIN_FUNCTION(append, 2),
        BUILTIN_FUNCTION(currentDirectory, 0),
        BUILTIN_FUNCTION(len, 1),
        BUILTIN_FUNCTION(sum, 1),
        BUILTIN_FUNCTION(product, 1)
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

std::shared_ptr<Object> BuiltinFunctions::builtin_input(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::string input;
    std::cin >> input;
    return std::make_shared<StringObject>(input);
}

std::shared_ptr<Object> BuiltinFunctions::builtin_read(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> filePathObj = arguments->at(0)->cast(TYPE_STRING);
    if (filePathObj == nullptr) {
        Error::runtimeError("Invalid argument to builtin read of type " + typeStrings[arguments->at(0)->getType()], line, col);
    }
    std::string filePath = std::static_pointer_cast<StringObject>(filePathObj)->getValue();
    std::ifstream stream(filePath);
    if (!stream.good()) {
        Error::runtimeError("Cannot access file from path '" + filePath + "'", line, col);
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string fileText = buffer.str();
    return std::make_shared<StringObject>(fileText);
}

std::shared_ptr<Object> BuiltinFunctions::builtin_write(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> filePathObj = arguments->at(0)->cast(TYPE_STRING);
    if (filePathObj == nullptr) {
        Error::runtimeError("Invalid argument to builtin write of type " + typeStrings[arguments->at(0)->getType()], line, col);
    }
    std::string filePath = std::static_pointer_cast<StringObject>(filePathObj)->getValue();
    std::shared_ptr<Object> fileTextObj = arguments->at(1)->cast(TYPE_STRING);
    if (fileTextObj == nullptr) {
        Error::runtimeError("Invalid argument to builtin write of type " + typeStrings[arguments->at(1)->getType()], line, col);
    }
    std::string fileText = std::static_pointer_cast<StringObject>(fileTextObj)->getValue();
    std::ofstream stream(filePath);
    stream << fileText;
    return std::make_shared<VoidObject>();
}

std::shared_ptr<Object> BuiltinFunctions::builtin_append(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> filePathObj = arguments->at(0)->cast(TYPE_STRING);
    if (filePathObj == nullptr) {
        Error::runtimeError("Invalid argument to builtin append of type " + typeStrings[arguments->at(0)->getType()], line, col);
    }
    std::string filePath = std::static_pointer_cast<StringObject>(filePathObj)->getValue();
    std::shared_ptr<Object> fileTextObj = arguments->at(1)->cast(TYPE_STRING);
    if (fileTextObj == nullptr) {
        Error::runtimeError("Invalid argument to builtin append of type " + typeStrings[arguments->at(1)->getType()], line, col);
    }
    std::string fileText = std::static_pointer_cast<StringObject>(fileTextObj)->getValue();
    std::ofstream stream(filePath, std::ios_base::app);
    stream << fileText;
    return std::make_shared<VoidObject>();
}

std::shared_ptr<Object> BuiltinFunctions::builtin_currentDirectory(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string cwdStr(cwd);
    return std::make_shared<StringObject>(cwdStr);
}

std::shared_ptr<Object> BuiltinFunctions::builtin_len(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    if (arguments->at(0)->getType() != TYPE_ARRAY && arguments->at(0)->getType() != TYPE_STRING) {
        Error::runtimeError("Invalid argument to builtin len of type " + typeStrings[arguments->at(0)->getType()], line, col);
    }

    if (arguments->at(0)->getType() == TYPE_ARRAY) {
        return std::make_shared<IntObject>(std::static_pointer_cast<ArrayObject>(arguments->at(0))->getLen());
    } else {
        return std::make_shared<IntObject>(std::static_pointer_cast<StringObject>(arguments->at(0))->getLen());
    }
}

std::shared_ptr<Object> BuiltinFunctions::builtin_sum(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    if (arguments->at(0)->getType() != TYPE_ARRAY) {
        Error::runtimeError("Invalid argument to builtin sum of type " + typeStrings[arguments->at(0)->getType()], line, col);
    }

    std::shared_ptr<Object> sum = std::make_shared<IntObject>(0);
    std::vector<std::shared_ptr<Object>> argValue = *std::static_pointer_cast<ArrayObject>(arguments->at(0))->getValue();
    for (auto &argument : argValue) {
        sum = sum->add(argument);
        if (sum == nullptr) {
            Error::runtimeError("Invalid argument to builtin sum of type " + typeStrings[argument->getType()], line, col);
        }
    }
    return sum;
}

std::shared_ptr<Object> BuiltinFunctions::builtin_product(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    if (arguments->at(0)->getType() != TYPE_ARRAY) {
        Error::runtimeError("Invalid argument to builtin product of type " + typeStrings[arguments->at(0)->getType()], line, col);
    }

    std::shared_ptr<Object> product = std::make_shared<IntObject>(1);
    std::vector<std::shared_ptr<Object>> argValue = *std::static_pointer_cast<ArrayObject>(arguments->at(0))->getValue();
    for (auto &argument : argValue) {
        product = product->mul(argument);
        if (product == nullptr) {
            Error::runtimeError("Invalid argument to builtin product of type " + typeStrings[argument->getType()], line, col);
        }
    }
    return product;
}
