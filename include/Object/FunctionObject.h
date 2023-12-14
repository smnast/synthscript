#ifndef SYNTHSCRIPT_FUNCTIONOBJECT_H
#define SYNTHSCRIPT_FUNCTIONOBJECT_H

#include "Object.h"
#include "AST/ASTNode.h"

class FunctionObject : public Object {
public:
    Type getType() override { return TYPE_FUNCTION; }
    FunctionObject(ASTNode *body, std::vector<std::string> parameters) : body(body), parameters(std::move(parameters)) {}
    ASTNode *getBody() { return body; }
    std::vector<std::string> *getParameters() { return &parameters; }
private:
    ASTNode *body;
    std::vector<std::string> parameters;
};

#endif //SYNTHSCRIPT_FUNCTIONOBJECT_H
