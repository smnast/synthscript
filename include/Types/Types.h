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

#endif //SYNTHSCRIPT_TYPES_H
