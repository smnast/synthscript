#ifndef SYNTHSCRIPT_OBJECT_H
#define SYNTHSCRIPT_OBJECT_H

#include "Types/Types.h"

class Object {
public:
    Object() = default;
    virtual Type getType() = 0;
};

#endif //SYNTHSCRIPT_OBJECT_H
