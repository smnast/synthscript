#ifndef SYNTHSCRIPT_OPERATORS_H
#define SYNTHSCRIPT_OPERATORS_H

#include "object/int_object.h"
#include "object/object.h"
#include <functional>
#include <utility>

std::function<std::shared_ptr<Object>(std::shared_ptr<Object>, std::shared_ptr<Object>)> get_binary_op_function(TokenType op) {
    switch (op) {
    case ADDITION_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->add(r); };
    case SUBTRACTION_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->subtract(r); };
    case MULTIPLICATIVE_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->multiply(r); };
    case DIVISION_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->divide(r); };
    case MOD_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->modulo(r); };
    case LOGICAL_AND_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->logical_and(r); };
    case LOGICAL_OR_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->logical_or(r); };
    case BITWISE_AND_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->bitwise_and(r); };
    case BITWISE_OR_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->bitwise_or(r); };
    case BITWISE_XOR_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->bitwise_xor(r); };
    case LESS_THAN_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->less_than(r); };
    case LESS_THAN_EQUAL_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->less_than_equal(r); };
    case GREATER_THAN_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return r->less_than(l); };
    case GREATER_THAN_EQUAL_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return r->less_than_equal(l); };
    case EQUAL_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->equal(r); };
    case NOT_EQUAL_OPERATOR:
        return [](const std::shared_ptr<Object> &l, const std::shared_ptr<Object> &r) -> std::shared_ptr<Object> { return l->not_equal(r); };
    default:
        return nullptr;
    }
}

std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)> get_unary_op_function(TokenType op) {
    switch (op) {
    case ADDITION_OPERATOR:
        return [](const std::shared_ptr<Object> &o) -> std::shared_ptr<Object> { return o->positive(); };
    case SUBTRACTION_OPERATOR:
        return [](const std::shared_ptr<Object> &o) -> std::shared_ptr<Object> { return o->negative(); };
    case LOGICAL_NOT_OPERATOR:
        return [](const std::shared_ptr<Object> &o) -> std::shared_ptr<Object> { return o->logical_not(); };
    case BITWISE_NOT_OPERATOR:
        return [](const std::shared_ptr<Object> &o) -> std::shared_ptr<Object> { return o->bitwise_not(); };
    default:
        return nullptr;
    }
}

#endif // SYNTHSCRIPT_OPERATORS_H
