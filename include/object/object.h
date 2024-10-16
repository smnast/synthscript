#ifndef SYNTHSCRIPT_OBJECT_H
#define SYNTHSCRIPT_OBJECT_H

#include "types/types.h"
#include <memory>

class Object {
public:
    Object() = default;
    virtual Type get_type() = 0;
    virtual std::shared_ptr<Object> add(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> subtract(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> positive() = 0;
    virtual std::shared_ptr<Object> negative() = 0;
    virtual std::shared_ptr<Object> multiply(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> divide(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> modulo(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_and(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_or(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_xor(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_not() = 0;
    virtual std::shared_ptr<Object> equal(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> not_equal(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> less_than(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> greater_than(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> less_than_equal(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> greater_than_equal(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> logical_and(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> logical_or(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> logical_not() = 0;
    virtual std::shared_ptr<Object> cast(Type type) = 0;
    virtual std::shared_ptr<Object> subscript(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> duplicate() = 0;
};

#endif // SYNTHSCRIPT_OBJECT_H
