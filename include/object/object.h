#ifndef SYNTHSCRIPT_OBJECT_H
#define SYNTHSCRIPT_OBJECT_H

#include "types/types.h"
#include <memory>

class Object {
public:
    Object() = default;
    virtual Type getType() = 0;
    virtual std::shared_ptr<Object> add(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> sub(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> positive() = 0;
    virtual std::shared_ptr<Object> negative() = 0;
    virtual std::shared_ptr<Object> mul(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> div(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> mod(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_and(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_or(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_xor(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> bitwise_not() = 0;
    virtual std::shared_ptr<Object> equal(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> notEqual(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> lessThan(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> greaterThan(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> lessThanEqual(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> greaterThanEqual(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> logicalAnd(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> logicalOr(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> logicalNot() = 0;
    virtual std::shared_ptr<Object> cast(Type type) = 0;
    virtual std::shared_ptr<Object> subscript(std::shared_ptr<Object> other) = 0;
    virtual std::shared_ptr<Object> duplicate() = 0;
};

#endif //SYNTHSCRIPT_OBJECT_H
