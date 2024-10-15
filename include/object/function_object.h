#ifndef SYNTHSCRIPT_FUNCTIONOBJECT_H
#define SYNTHSCRIPT_FUNCTIONOBJECT_H

#include "object.h"
#include "AST/AST_node.h"

class FunctionObject : public Object {
public:
    FunctionObject(ASTNode *body, std::vector<std::string> parameters, bool builtin=false) : body(body), parameters(std::move(parameters)), builtin(builtin) {}
    Type getType() override { return TYPE_FUNCTION; }
    std::shared_ptr<Object> add(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> sub(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> positive() override;
    std::shared_ptr<Object> negative() override;
    std::shared_ptr<Object> mul(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> div(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> mod(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_and(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_or(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_xor(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_not() override;
    std::shared_ptr<Object> equal(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> notEqual(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> lessThan(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> greaterThan(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> lessThanEqual(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> greaterThanEqual(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logicalAnd(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logicalOr(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logicalNot() override;
    std::shared_ptr<Object> cast(Type type) override;
    std::shared_ptr<Object> subscript(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> duplicate() override;
    ASTNode *getBody() { return body; }
    std::vector<std::string> *getParameters() { return &parameters; }
    bool isBuiltin() const { return builtin; }
private:
    ASTNode *body;
    std::vector<std::string> parameters;
    bool builtin;
};

#endif //SYNTHSCRIPT_FUNCTIONOBJECT_H
