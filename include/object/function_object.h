#ifndef SYNTHSCRIPT_FUNCTIONOBJECT_H
#define SYNTHSCRIPT_FUNCTIONOBJECT_H

#include "AST/AST_node.h"
#include "object.h"

class FunctionObject : public Object {
public:
    FunctionObject(ASTNode *body, std::vector<std::string> parameters, bool builtin = false)
        : body(body), parameters(std::move(parameters)), built_in(builtin) {}

    Type get_type() override { return TYPE_FUNCTION; }

    std::shared_ptr<Object> add(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> subtract(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> positive() override;
    std::shared_ptr<Object> negative() override;
    std::shared_ptr<Object> multiply(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> divide(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> modulo(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_and(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_or(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_xor(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> bitwise_not() override;
    std::shared_ptr<Object> equal(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> not_equal(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> less_than(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> greater_than(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> less_than_equal(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> greater_than_equal(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logical_and(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logical_or(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logical_not() override;
    std::shared_ptr<Object> cast(Type type) override;
    std::shared_ptr<Object> subscript(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> duplicate() override;

    ASTNode *get_body() { return body; }
    std::vector<std::string> *get_parameters() { return &parameters; }
    bool is_built_in() const { return built_in; }

private:
    ASTNode *body;
    std::vector<std::string> parameters;
    bool built_in;
};

#endif // SYNTHSCRIPT_FUNCTIONOBJECT_H
