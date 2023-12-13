#ifndef CLIKE_ASTNODE_H
#define CLIKE_ASTNODE_H

#include <iostream>

#include "Error.h"
#include "VisitFunctionsMacro.h"
#include "Types/Types.h"

class ASTNode {
public:
    explicit ASTNode(int lineNumber) : lineNumber(lineNumber) {};
    virtual ~ASTNode() = default;
    int getLineNumber() const { return lineNumber; }
private:
    int lineNumber;
};


#endif //CLIKE_ASTNODE_H
