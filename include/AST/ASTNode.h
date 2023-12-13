#ifndef CLIKE_ASTNODE_H
#define CLIKE_ASTNODE_H

#include <iostream>

#include "Error.h"
#include "VisitFunctionsMacro.h"
#include "Types/Types.h"

class ASTNode {
public:
    explicit ASTNode(int line, int col) : line(line), col(col) {};
    virtual ~ASTNode() = default;
    int getLineNumber() const { return line; }
    int getColumnNumber() const { return col; }
private:
    int line, col;
};


#endif //CLIKE_ASTNODE_H
