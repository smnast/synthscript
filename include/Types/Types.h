#ifndef SYNTHSCRIPT_TYPES_H
#define SYNTHSCRIPT_TYPES_H

#include <string>
#include "Tokens.h"

enum Type {
    TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VOID, TYPE_ARRAY, TYPE_FUNCTION, TYPE_UNDEF
};

static std::string typeStrings[] = {
        "int", "float", "bool", "string", "void", "array", "function", "<error>"
};

static Type tokenToType(TokenType type) {
    switch (type) {
        case INT_TYPE:
            return TYPE_INT;
        case FLOAT_TYPE:
            return TYPE_FLOAT;
        case BOOL_TYPE:
            return TYPE_BOOL;
        case STRING_TYPE:
            return TYPE_STRING;
        case VOID_TYPE:
            return TYPE_VOID;
        case ARRAY_TYPE:
            return TYPE_ARRAY;
        default:
            return TYPE_UNDEF;
    }
}

static bool tokenIsType(TokenType type) {
    return type == INT_TYPE || type == FLOAT_TYPE || type == BOOL_TYPE || type == STRING_TYPE || type == VOID_TYPE || type == ARRAY_TYPE;
}

static bool tokenIsLiteral(TokenType type) {
    return type == INT_LITERAL || type == FLOAT_LITERAL || type == BOOL_LITERAL || type == STRING_LITERAL;
}

#endif //SYNTHSCRIPT_TYPES_H
