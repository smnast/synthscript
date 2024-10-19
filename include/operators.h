#ifndef SYNTHSCRIPT_OPERATORS_H
#define SYNTHSCRIPT_OPERATORS_H

#include "object/object.h"
#include <functional>

/**
 * Get the binary operator function for the given operator.
 * @param op The operator.
 * @return The binary operator function.
 */
std::function<std::shared_ptr<Object>(std::shared_ptr<Object>, std::shared_ptr<Object>)>
get_binary_op_function(TokenType op);

/**
 * Get the unary operator function for the given operator.
 * @param op The operator.
 * @return The unary operator function.
 */
std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)> get_unary_op_function(TokenType op);

#endif // SYNTHSCRIPT_OPERATORS_H
