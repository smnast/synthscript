#ifndef SYNTHSCRIPT_TYPES_H
#define SYNTHSCRIPT_TYPES_H

#include "tokens.h"
#include <string>

enum Type {
    TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VOID, TYPE_ARRAY, TYPE_FUNCTION, TYPE_UNDEF
};

static std::string typeStrings[] = {
        "int", "float", "bool", "string", "void", "array", "function", "<error>"
};

static Type token_to_type(TokenType type) {
    switch (type) {
        case INT_TYPE:
        case INT_LITERAL:
            return TYPE_INT;
        case FLOAT_TYPE:
        case FLOAT_LITERAL:
            return TYPE_FLOAT;
        case BOOL_TYPE:
        case BOOL_LITERAL:
            return TYPE_BOOL;
        case STRING_TYPE:
        case STRING_LITERAL:
            return TYPE_STRING;
        case ARRAY_TYPE:
            return TYPE_ARRAY;
        case VOID_TYPE:
            return TYPE_VOID;
        default:
            return TYPE_UNDEF;
    }
}

#endif //SYNTHSCRIPT_TYPES_H
