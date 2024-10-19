#include "built_in_functions.h"
#include "error.h"
#include "object/array_object.h"
#include "object/function_object.h"
#include "object/int_object.h"
#include "object/string_object.h"
#include "object/void_object.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

#define BUILT_IN_FUNCTION(name, param_count)                                                       \
    {                                                                                              \
        #name, {                                                                                   \
            [](std::vector<std::shared_ptr<Object>> arguments,                                     \
               int line,                                                                           \
               int col) -> std::shared_ptr<Object> {                                               \
                return BuiltInFunctions::built_in_##name(&arguments, line, col);                   \
            },                                                                                     \
                param_count                                                                        \
        }                                                                                          \
    }

std::unordered_map<std::string, BuiltInFunction> BuiltInFunctions::built_in_functions = {
    BUILT_IN_FUNCTION(output, 1),
    BUILT_IN_FUNCTION(input, 0),
    BUILT_IN_FUNCTION(read, 1),
    BUILT_IN_FUNCTION(write, 2),
    BUILT_IN_FUNCTION(append, 2),
    BUILT_IN_FUNCTION(current_directory, 0),
    BUILT_IN_FUNCTION(len, 1),
    BUILT_IN_FUNCTION(sum, 1),
    BUILT_IN_FUNCTION(product, 1)};

void BuiltInFunctions::register_built_in_functions(SymbolTable *symbol_table) {
    for (const auto &built_in_function : built_in_functions) {
        std::vector<std::string> parameters(built_in_function.second.param_count);
        std::shared_ptr<Object> function_object =
            std::make_shared<FunctionObject>(nullptr, parameters, true);
        Symbol function_symbol(built_in_function.first, function_object);
        symbol_table->insert(function_symbol);
    }
}

std::shared_ptr<Object>
BuiltInFunctions::handle_built_in_function(const std::string &identifier,
                                           std::vector<std::shared_ptr<Object>> *arguments,
                                           int line,
                                           int col) {
    return built_in_functions[identifier].function(*arguments, line, col);
}

std::shared_ptr<Object> BuiltInFunctions::built_in_output(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> cast_obj = arguments->at(0)->cast(TYPE_STRING);
    if (cast_obj == nullptr) {
        Error::runtime_error("Invalid argument to built-in output of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    } else {
        printf("%s\n", std::static_pointer_cast<StringObject>(cast_obj)->get_value().c_str());
    }

    return std::make_shared<VoidObject>();
}

std::shared_ptr<Object> BuiltInFunctions::built_in_input(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::string input;
    std::cin >> input;
    return std::make_shared<StringObject>(input);
}

std::shared_ptr<Object> BuiltInFunctions::built_in_read(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> file_path_obj = arguments->at(0)->cast(TYPE_STRING);
    if (file_path_obj == nullptr) {
        Error::runtime_error("Invalid argument to built-in read of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    }
    std::string file_path = std::static_pointer_cast<StringObject>(file_path_obj)->get_value();
    std::ifstream stream(file_path);
    if (!stream.good()) {
        Error::runtime_error("Cannot access file from path '" + file_path + "'", line, col);
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string file_text = buffer.str();
    return std::make_shared<StringObject>(file_text);
}

std::shared_ptr<Object> BuiltInFunctions::built_in_write(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> file_path_obj = arguments->at(0)->cast(TYPE_STRING);
    if (file_path_obj == nullptr) {
        Error::runtime_error("Invalid argument to built-in write of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    }
    std::string file_path = std::static_pointer_cast<StringObject>(file_path_obj)->get_value();
    std::shared_ptr<Object> file_text_obj = arguments->at(1)->cast(TYPE_STRING);
    if (file_text_obj == nullptr) {
        Error::runtime_error("Invalid argument to built-in write of type " +
                                 type_strings[arguments->at(1)->get_type()],
                             line,
                             col);
    }
    std::string file_text = std::static_pointer_cast<StringObject>(file_text_obj)->get_value();
    std::ofstream stream(file_path);
    stream << file_text;
    return std::make_shared<VoidObject>();
}

std::shared_ptr<Object> BuiltInFunctions::built_in_append(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    std::shared_ptr<Object> file_path_obj = arguments->at(0)->cast(TYPE_STRING);
    if (file_path_obj == nullptr) {
        Error::runtime_error("Invalid argument to built-in append of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    }
    std::string file_path = std::static_pointer_cast<StringObject>(file_path_obj)->get_value();
    std::shared_ptr<Object> file_text_obj = arguments->at(1)->cast(TYPE_STRING);
    if (file_text_obj == nullptr) {
        Error::runtime_error("Invalid argument to built-in append of type " +
                                 type_strings[arguments->at(1)->get_type()],
                             line,
                             col);
    }
    std::string file_text = std::static_pointer_cast<StringObject>(file_text_obj)->get_value();
    std::ofstream stream(file_path, std::ios_base::app);
    stream << file_text;
    return std::make_shared<VoidObject>();
}

std::shared_ptr<Object> BuiltInFunctions::built_in_current_directory(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::string cwd_str(cwd);
    return std::make_shared<StringObject>(cwd_str);
}

std::shared_ptr<Object>
BuiltInFunctions::built_in_len(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    if (arguments->at(0)->get_type() != TYPE_ARRAY && arguments->at(0)->get_type() != TYPE_STRING) {
        Error::runtime_error("Invalid argument to built-in len of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    }

    if (arguments->at(0)->get_type() == TYPE_ARRAY) {
        return std::make_shared<IntObject>(
            std::static_pointer_cast<ArrayObject>(arguments->at(0))->get_len());
    } else {
        return std::make_shared<IntObject>(
            std::static_pointer_cast<StringObject>(arguments->at(0))->get_len());
    }
}

std::shared_ptr<Object>
BuiltInFunctions::built_in_sum(std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    if (arguments->at(0)->get_type() != TYPE_ARRAY) {
        Error::runtime_error("Invalid argument to built-in sum of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    }

    std::shared_ptr<Object> sum = std::make_shared<IntObject>(0);
    std::vector<std::shared_ptr<Object>> arg_value =
        *std::static_pointer_cast<ArrayObject>(arguments->at(0))->get_value();
    for (auto &argument : arg_value) {
        sum = sum->add(argument);
        if (sum == nullptr) {
            Error::runtime_error("Invalid argument to built-in sum of type " +
                                     type_strings[argument->get_type()],
                                 line,
                                 col);
        }
    }
    return sum;
}

std::shared_ptr<Object> BuiltInFunctions::built_in_product(
    std::vector<std::shared_ptr<Object>> *arguments, int line, int col) {
    if (arguments->at(0)->get_type() != TYPE_ARRAY) {
        Error::runtime_error("Invalid argument to built-in product of type " +
                                 type_strings[arguments->at(0)->get_type()],
                             line,
                             col);
    }

    std::shared_ptr<Object> product = std::make_shared<IntObject>(1);
    std::vector<std::shared_ptr<Object>> arg_value =
        *std::static_pointer_cast<ArrayObject>(arguments->at(0))->get_value();
    for (auto &argument : arg_value) {
        product = product->multiply(argument);
        if (product == nullptr) {
            Error::runtime_error("Invalid argument to built-in product of type " +
                                     type_strings[argument->get_type()],
                                 line,
                                 col);
        }
    }
    return product;
}
