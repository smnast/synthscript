#ifndef SYNTHSCRIPT_ARRAYOBJECT_H
#define SYNTHSCRIPT_ARRAYOBJECT_H

#include "object.h"
#include <utility>

class ArrayObject : public Object {
public:
    explicit ArrayObject(std::vector<std::shared_ptr<Object>> value) : value(std::move(value)) {}

    Type get_type() override { return TYPE_ARRAY; }

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
    std::shared_ptr<Object> subscript_update(const std::shared_ptr<Object> &index,
                                             const std::shared_ptr<Object> &val);
    std::shared_ptr<Object> duplicate() override;
    std::shared_ptr<Object> call(InterpreterVisitor *visitor, SymbolTable *table) override;

    int get_len() const { return (int)value.size(); };
    std::vector<std::shared_ptr<Object>> *get_value() { return &value; }

private:
    std::vector<std::shared_ptr<Object>> value;
};

#endif // SYNTHSCRIPT_ARRAYOBJECT_H
