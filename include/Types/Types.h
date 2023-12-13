#ifndef SYNTHSCRIPT_TYPES_H
#define SYNTHSCRIPT_TYPES_H

#include <string>

enum Type {
    TYPE_INT, TYPE_FLOAT, TYPE_BOOL, TYPE_STRING, TYPE_VOID, TYPE_ARRAY, TYPE_UNDEF
};

static std::string typeStrings[] = {
        "int", "float", "bool", "string", "void", "array", "<error>"
};

#endif //SYNTHSCRIPT_TYPES_H
