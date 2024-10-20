#include "types/types.h"

std::string type_to_string(Type type) {
    std::string type_names[]{
        "int", "float", "bool", "string", "void", "array", "function", "<error>"};
    return type_names[type];
}

Type token_to_type(TokenType type) {
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
