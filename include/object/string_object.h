#ifndef SYNTHSCRIPT_STRINGOBJECT_H
#define SYNTHSCRIPT_STRINGOBJECT_H

#include "object.h"

class StringObject : public Object {
public:
    explicit StringObject(std::string value) : value(value) {}
    Type get_type() override { return TYPE_STRING; }
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
    int get_len() const { return (int)value.size(); };
    std::string get_value() { return value; }
private:
    std::string value;
};

#endif //SYNTHSCRIPT_STRINGOBJECT_H
