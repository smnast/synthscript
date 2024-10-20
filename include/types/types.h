#ifndef SYNTHSCRIPT_TYPES_H
#define SYNTHSCRIPT_TYPES_H

#include "tokens.h"
#include <string>

enum Type {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_STRING,
    TYPE_VOID,
    TYPE_ARRAY,
    TYPE_FUNCTION,
    TYPE_UNDEF
};

/**
 * @brief Converts a type to a string name.
 * @param type The type.
 * @return The string name of the type.
 */
std::string type_to_string(Type type);

/**
 * @brief Converts a token type to a type.
 * @param type The token type.
 * @return The type.
 */
Type token_to_type(TokenType type);

#endif // SYNTHSCRIPT_TYPES_H
