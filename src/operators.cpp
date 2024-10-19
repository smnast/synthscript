#include "operators.h"

const std::unordered_map<TokenType, BinaryOp> binary_ops = {
    {ADDITION_OPERATOR, [](auto l, auto r) { return l->add(r); }},
    {SUBTRACTION_OPERATOR, [](auto l, auto r) { return l->subtract(r); }},
    {MULTIPLICATIVE_OPERATOR, [](auto l, auto r) { return l->multiply(r); }},
    {DIVISION_OPERATOR, [](auto l, auto r) { return l->divide(r); }},
    {MOD_OPERATOR, [](auto l, auto r) { return l->modulo(r); }},
    {LOGICAL_AND_OPERATOR, [](auto l, auto r) { return l->logical_and(r); }},
    {LOGICAL_OR_OPERATOR, [](auto l, auto r) { return l->logical_or(r); }},
    {BITWISE_AND_OPERATOR, [](auto l, auto r) { return l->bitwise_and(r); }},
    {BITWISE_OR_OPERATOR, [](auto l, auto r) { return l->bitwise_or(r); }},
    {BITWISE_XOR_OPERATOR, [](auto l, auto r) { return l->bitwise_xor(r); }},
    {LESS_THAN_OPERATOR, [](auto l, auto r) { return l->less_than(r); }},
    {LESS_THAN_EQUAL_OPERATOR, [](auto l, auto r) { return l->less_than_equal(r); }},
    {GREATER_THAN_OPERATOR, [](auto l, auto r) { return r->less_than(l); }},
    {GREATER_THAN_EQUAL_OPERATOR, [](auto l, auto r) { return r->less_than_equal(l); }},
    {EQUAL_OPERATOR, [](auto l, auto r) { return l->equal(r); }},
    {NOT_EQUAL_OPERATOR, [](auto l, auto r) { return l->not_equal(r); }}
};

const std::unordered_map<TokenType, UnaryOp> unary_ops = {
    {ADDITION_OPERATOR, [](auto o) { return o->positive(); }},
    {SUBTRACTION_OPERATOR, [](auto o) { return o->negative(); }},
    {LOGICAL_NOT_OPERATOR, [](auto o) { return o->logical_not(); }},
    {BITWISE_NOT_OPERATOR, [](auto o) { return o->bitwise_not(); }}
};

BinaryOp get_binary_op_function(TokenType op) {
    auto it = binary_ops.find(op);
    if (it != binary_ops.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

UnaryOp get_unary_op_function(TokenType op) {
    auto it = unary_ops.find(op);
    if (it == unary_ops.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}
