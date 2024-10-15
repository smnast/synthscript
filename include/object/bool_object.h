#ifndef SYNTHSCRIPT_BOOLOBJECT_H
#define SYNTHSCRIPT_BOOLOBJECT_H

#include "object.h"

class BoolObject : public Object {
public:
    explicit BoolObject(bool value) : value(value) {}
    Type getType() override { return TYPE_BOOL; }
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
    bool getValue() const { return value; }
private:
    bool value;
};

#endif //SYNTHSCRIPT_BOOLOBJECT_H
