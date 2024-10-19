#ifndef SYNTHSCRIPT_OPERATORS_H
#define SYNTHSCRIPT_OPERATORS_H

#include "object/object.h"
#include <functional>

// Type alias for readability
using BinaryOp =
    std::function<std::shared_ptr<Object>(std::shared_ptr<Object>, std::shared_ptr<Object>)>;
using UnaryOp = std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)>;

/**
 * Get the binary operator function for the given operator.
 * @param op The operator.
 * @return The binary operator function.
 */
BinaryOp get_binary_op_function(TokenType op);

/**
 * Get the unary operator function for the given operator.
 * @param op The operator.
 * @return The unary operator function.
 */
UnaryOp get_unary_op_function(TokenType op);

#endif // SYNTHSCRIPT_OPERATORS_H
