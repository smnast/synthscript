#include "object/bool_object.h"
#include "object/float_object.h"
#include "object/int_object.h"
#include "object/string_object.h"

std::shared_ptr<Object> BoolObject::add(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::subtract(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::positive() {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::negative() {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::multiply(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::divide(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::modulo(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::bitwise_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::bitwise_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::bitwise_xor(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::bitwise_not() {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::equal(std::shared_ptr<Object> other) {
    return std::make_shared<BoolObject>(value ==
                                        std::static_pointer_cast<BoolObject>(other)->get_value());
}

std::shared_ptr<Object> BoolObject::not_equal(std::shared_ptr<Object> other) {
    return std::make_shared<BoolObject>(value !=
                                        std::static_pointer_cast<BoolObject>(other)->get_value());
}

std::shared_ptr<Object> BoolObject::less_than(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::greater_than(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::less_than_equal(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::greater_than_equal(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::logical_and(std::shared_ptr<Object> other) {
    return std::make_shared<BoolObject>(value &&
                                        std::static_pointer_cast<BoolObject>(other)->get_value());
}

std::shared_ptr<Object> BoolObject::logical_or(std::shared_ptr<Object> other) {
    return std::make_shared<BoolObject>(value ||
                                        std::static_pointer_cast<BoolObject>(other)->get_value());
}

std::shared_ptr<Object> BoolObject::logical_not() {
    return std::make_shared<BoolObject>(!value);
}

std::shared_ptr<Object> BoolObject::cast(Type type) {
    if (type == TYPE_BOOL) {
        return std::make_shared<BoolObject>(value);
    } else if (type == TYPE_INT) {
        return std::make_shared<IntObject>(value ? 1 : 0);
    } else if (type == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(value ? 1.0f : 0.0f);
    } else if (type == TYPE_STRING) {
        return std::make_shared<StringObject>(value ? "true" : "false");
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> BoolObject::subscript(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> BoolObject::duplicate() {
    return std::make_shared<BoolObject>(value);
}

std::shared_ptr<Object> BoolObject::call(InterpreterVisitor *visitor, SymbolTable *table) {
    return nullptr;
}
