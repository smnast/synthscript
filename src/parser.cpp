#include "parser.h"

Parser::Parser(std::vector<Token> program_tokens, ErrorManager *error_manager)
    : program_tokens(std::move(program_tokens)), error_manager(error_manager) {}

ProgramNode *Parser::parse_program() {
    cur_idx = 0;
    tokens = std::move(program_tokens);

    // Parse the program tokens
    std::vector<ASTNode *> statements;
    while (!check(END_OF_FILE)) {
        // If there is an error, handle it before continuing parsing
        if (error_manager->check_error()) {
            sync();
        } else {
            auto *statement_node = parse_statement();
            statements.push_back(statement_node);
            accept_new_lines();
        }
    }

    return new ProgramNode(statements, 0, 0);
}

ASTNode *Parser::parse_statement() {
    // TODO: try to remove this!
    accept_new_lines();

    // Parse the statement
    ASTNode *node = nullptr;
    if (check(IF_KEYWORD)) {
        node = parse_if_statement();
    } else if (check(WHILE_KEYWORD)) {
        node = parse_while_statement();
    } else if (check(FOR_KEYWORD)) {
        node = parse_for_statement();
    } else if (check(REPEAT_KEYWORD)) {
        node = parse_repeat_statement();
    } else if (check(BREAK_KEYWORD)) {
        node = parse_break_statement();
    } else if (check(CONTINUE_KEYWORD)) {
        node = parse_continue_statement();
    } else if (check(RETURN_KEYWORD)) {
        node = parse_return_statement();
    } else if (check(LBRACE)) {
        node = parse_compound_statement();
    } else if (!check(END_OF_FILE) && !check(RBRACE)) {
        node = parse_primary_expression();
    } else {
        // If the current token is not a statement, throw a syntax error
        syntax_error("statement", cur_token());
        return new ErrorNode(cur_token().line, cur_token().column);
    }

    return node;
}

ASTNode *Parser::parse_compound_statement() {
    /*
        Example:
        {
            statement1
            statement2
        }
    */

    int line = cur_token().line, col = cur_token().column;

    expect(LBRACE);
    accept_new_lines();

    // Statements inside the compound statement
    std::vector<ASTNode *> statements;
    while (!check(RBRACE) && !check(END_OF_FILE)) {
        // If there is an error, handle it before continuing parsing
        // TODO: remove repeated code! (from parse program function)
        if (error_manager->check_error()) {
            sync();
        } else {
            auto *statement_node = parse_statement();
            statements.push_back(statement_node);
            accept_new_lines();
        }
    }

    expect(RBRACE);

    return new CompoundStatementNode(statements, line, col);
}

ASTNode *Parser::parse_array_literal() {
    /*
        Example:
        [value1, value2, value3]
    */

    int line = cur_token().line, col = cur_token().column;

    expect(LBRACKET);

    // List of comma-separated values within the brackets
    std::vector<ASTNode *> values;
    while (!check(RBRACKET) && !check(END_OF_FILE)) {
        // Parse the primary expression as a value
        auto *value = parse_primary_expression();
        values.push_back(value);

        // Require comma if there are more values
        if (!accept(COMMA)) {
            break;
        }
    }

    expect(RBRACKET);

    return new ArrayLiteralNode(values, line, col);
}

ASTNode *Parser::parse_array_subscript() {
    /*
        Example:
        identifier[index1][index2]
    */

    ASTNode *left_array = parse_identifier();

    // List of square brackets with index expressions, ordered left to right
    while (accept(LBRACKET)) {
        // Parse the primary expression as an index
        int line = cur_token().line, col = cur_token().column;
        auto *index = parse_primary_expression();

        // left_array becomes the left array at the specified index
        left_array = new SubscriptOpNode(left_array, index, line, col);

        expect(RBRACKET);
    }

    return left_array;
}

ASTNode *Parser::parse_cast() {
    /*
        Example:
        type(value)
    */

    int line = cur_token().line, col = cur_token().column;

    if (token_is_type(cur_token().type)) {
        TokenType token_type = cur_token().type;
        next_token();

        expect(LPAREN);
        auto *value = parse_primary_expression();
        expect(RPAREN);

        return new CastOpNode(token_to_type(token_type), value, line, col);
    } else {
        // If the current token is not a type, throw a syntax error
        syntax_error("type", cur_token());
        return new ErrorNode(cur_token().line, cur_token().column);
    }
}

ASTNode *Parser::parse_if_statement() {
    /*
        Example:
        if condition {
            statement1
            statement2
        } else {
            statement3
            statement4
        }
    */

    int line = cur_token().line, col = cur_token().column;

    expect(IF_KEYWORD);
    auto *condition = parse_primary_expression();
    auto *if_body = parse_compound_statement();

    ASTNode *else_body = nullptr;
    if (accept(ELSE_KEYWORD)) {
        // Allow compounding else with another if statment
        if (check(IF_KEYWORD)) {
            else_body = parse_if_statement();
            // Otherwise, parse the else body in a compound statement
        } else {
            else_body = parse_compound_statement();
        }
    }

    return new IfStatementNode(condition, if_body, else_body, line, col);
}

ASTNode *Parser::parse_while_statement() {
    /*
        Example:
        while condition {
            statement1
            statement2
        }
    */

    int line = cur_token().line, col = cur_token().column;

    expect(WHILE_KEYWORD);
    auto *condition = parse_primary_expression();
    auto *body = parse_compound_statement();

    return new WhileStatementNode(condition, body, line, col);
}

ASTNode *Parser::parse_for_statement() {
    /*
        Example:
        for identifier in iterable {
            statment
        }
    */

    int line = cur_token().line, col = cur_token().column;

    expect(FOR_KEYWORD);
    std::string identifier = cur_token().value;
    expect(IDENTIFIER);
    expect(IN_KEYWORD);
    auto *iterable = parse_primary_expression();
    auto *body = parse_compound_statement();

    return new ForStatementNode(identifier, iterable, body, line, col);
}

ASTNode *Parser::parse_repeat_statement() {
    /*
        Example:
        repeat count {
            statement1
            statement2
        }
    */

    int line = cur_token().line, col = cur_token().column;

    expect(REPEAT_KEYWORD);
    auto *count = parse_primary_expression();
    auto *body = parse_compound_statement();

    return new RepeatStatementNode(count, body, line, col);
}

ASTNode *Parser::parse_break_statement() {
    /*
        Example:
        break
    */

    int line = cur_token().line, col = cur_token().column;

    expect(BREAK_KEYWORD);

    return new BreakStatementNode(line, col);
}

ASTNode *Parser::parse_continue_statement() {
    /*
        Example:
        continue
    */

    int line = cur_token().line, col = cur_token().column;

    expect(CONTINUE_KEYWORD);

    return new ContinueStatementNode(line, col);
}

ASTNode *Parser::parse_return_statement() {
    /*
        Example:
        return value
    */

    int line = cur_token().line, col = cur_token().column;

    expect(RETURN_KEYWORD);

    // Value (if it exists)
    ASTNode *value = nullptr;
    if (!check(END_OF_FILE) && !check(NEW_LINE) && !check(RBRACE)) {
        value = parse_primary_expression();
    }

    return new ReturnStatementNode(value, line, col);
}

ASTNode *Parser::parse_identifier() {
    /*
        Example:
        identifier
    */

    int line = cur_token().line, col = cur_token().column;

    std::string identifier = cur_token().value;
    expect(IDENTIFIER);

    return new IdentifierNode(identifier, line, col);
}

ASTNode *Parser::parse_literal() {
    /*
        Examples:
        123
        123.456
        "string"
        true
    */

    int line = cur_token().line, col = cur_token().column;

    // Parse the literal value
    Type type = token_to_type(cur_token().type);
    std::string value = cur_token().value;
    next_token();

    return new LiteralNode(type, value, line, col);
}

ASTNode *Parser::parse_function_declaration() {
    /*
        Example:
        function(parameter1, parameter2) {
            statement1
            statement2
        }
    */

    int line = cur_token().line, col = cur_token().column;

    expect(FUNCTION_KEYWORD);
    expect(LPAREN);

    // List of comma-separated parameters within the parentheses
    std::vector<std::string> parameters;
    while (!check(RPAREN) && !check(END_OF_FILE)) {
        // Parse the identifier as a parameter
        std::string parameter = cur_token().value;
        expect(IDENTIFIER);
        parameters.push_back(parameter);

        // Require comma if there are more parameters
        if (!accept(COMMA)) {
            break;
        }
    }

    expect(RPAREN);
    ASTNode *body = parse_compound_statement();

    return new FunctionDeclarationNode(parameters, body, line, col);
}

ASTNode *Parser::parse_call() {
    /*
        Example:
        identifier(argument1, argument2)
    */

    int line = cur_token().line, col = cur_token().column;

    std::string identifier = cur_token().value;
    expect(IDENTIFIER);
    expect(LPAREN);

    // List of comma-separated arguments within the parentheses
    std::vector<ASTNode *> arguments;
    while (!check(RPAREN) && !check(END_OF_FILE)) {
        // Parse the primary expression as an argument
        auto *argument = parse_primary_expression();
        arguments.push_back(argument);

        // Require comma if there are more arguments
        if (!accept(COMMA)) {
            break;
        }
    }

    expect(RPAREN);

    return new CallOpNode(identifier, arguments, line, col);
}

ASTNode *Parser::parse_primary_expression() {
    // Highest level expression.
    // This is just a wrapper for the next level of expression.
    return parse_assignment_expression();
}

ASTNode *Parser::parse_assignment_expression() {
    /*
        Example:
        left <- right
    */

    int line = cur_token().line, col = cur_token().column;

    ASTNode *left = parse_logical_or_expression();

    if (accept(ASSIGNMENT_OPERATOR)) {
        // Align to the right
        auto *right = parse_assignment_expression();
        left = new AssignmentNode(left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_logical_or_expression() {
    /*
        Example:
        left or right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_logical_and_expression();
    while (check(LOGICAL_OR_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(LOGICAL_OR_OPERATOR);

        auto *right = parse_logical_and_expression();
        left = new BinOpNode(op, left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_logical_and_expression() {
    /*
        Example:
        left and right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_bitwise_or_expression();
    while (check(LOGICAL_AND_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(LOGICAL_AND_OPERATOR);

        auto *right = parse_bitwise_or_expression();
        left = new BinOpNode(op, left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_bitwise_or_expression() {
    /*
        Example:
        left | right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_bitwise_xor_expression();
    while (check(BITWISE_OR_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(BITWISE_OR_OPERATOR);

        auto *right = parse_bitwise_xor_expression();
        left = new BinOpNode(op, left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_bitwise_xor_expression() {
    /*
        Example:
        left ^ right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_bitwise_and_expression();
    while (check(BITWISE_XOR_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(BITWISE_XOR_OPERATOR);

        auto *right = parse_bitwise_and_expression();
        left = new BinOpNode(op, left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_bitwise_and_expression() {
    /*
        Example:
        left & right
    */

    // TODO: lot of repetition. Simplify grammar to have a only few of these functions.
    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_equality_expression();
    while (check(BITWISE_AND_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(BITWISE_AND_OPERATOR);

        auto *right = parse_equality_expression();
        left = new BinOpNode(op, left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_equality_expression() {
    /*
        Examples:
        left = right
        left != right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_relational_expression();
    while (check(EQUAL_OPERATOR) || check(NOT_EQUAL_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(cur_token().type);

        auto *right = parse_relational_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_relational_expression() {
    /*
        Examples:
        left < right
        left <= right
        left > right
        left >= right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_range_literal_expression();
    while (check(LESS_THAN_OPERATOR) || check(LESS_THAN_EQUAL_OPERATOR) ||
           check(GREATER_THAN_OPERATOR) || check(GREATER_THAN_EQUAL_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(cur_token().type);

        auto *right = parse_range_literal_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_range_literal_expression() {
    /*
        Examples:
        left..right
    */

    int line = cur_token().line, col = cur_token().column;

    auto *left = parse_additive_expression();
    if (accept(RANGE_SYMBOL)) {
        auto *right = parse_additive_expression();
        left = new RangeLiteralNode(left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_additive_expression() {
    /*
        Examples:
        left + right
        left - right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_multiplicative_expression();
    while (check(ADDITION_OPERATOR) || check(SUBTRACTION_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(cur_token().type);

        auto *right = parse_multiplicative_expression();
        left = new BinOpNode(op, left, right, line, col);
    }

    return left;
}

ASTNode *Parser::parse_multiplicative_expression() {
    /*
        Examples:
        left * right
        left / right
        left % right
    */

    int line = cur_token().line, col = cur_token().column;

    // Align to the left
    auto *left = parse_unary_expression();
    while (check(MULTIPLICATIVE_OPERATOR) || check(DIVISION_OPERATOR) || check(MOD_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(cur_token().type);

        auto *right = parse_unary_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_unary_expression() {
    /*
        Examples:
        -value
        +value
        ~value
        !value
    */

    int line = cur_token().line, col = cur_token().column;

    if (check(LOGICAL_NOT_OPERATOR) || check(BITWISE_NOT_OPERATOR) || check(SUBTRACTION_OPERATOR) ||
        check(ADDITION_OPERATOR)) {
        TokenType op = cur_token().type;
        accept(cur_token().type);

        auto *right = parse_factor_expression();
        return new UnaryOpNode(op, right, line, col);
    } else {
        return parse_factor_expression();
    }
}

ASTNode *Parser::parse_factor_expression() {
    /*
        Examples:
        (expression)
        identifier
        ...etc.
    */

    if (token_is_type(cur_token().type)) {
        return parse_cast();
    } else if (check(FUNCTION_KEYWORD)) {
        return parse_function_declaration();
    } else if (check(IDENTIFIER) && peek_token(LPAREN, 1)) {
        return parse_call();
    } else if (check(IDENTIFIER) && peek_token(LBRACKET, 1)) {
        return parse_array_subscript();
    } else if (check(IDENTIFIER)) {
        return parse_identifier();
    } else if (check(LPAREN)) {
        expect(LPAREN);
        auto *exp = parse_primary_expression();
        expect(RPAREN);
        return exp;
    } else if (check(LBRACKET)) {
        return parse_array_literal();
    } else if (check(INT_LITERAL) || check(FLOAT_LITERAL) || check(STRING_LITERAL) ||
               check(BOOL_LITERAL)) {
        return parse_literal();
    } else {
        // If the current token is not a factor, throw a syntax error
        syntax_error("expression", cur_token());
        return new ErrorNode(cur_token().line, cur_token().column);
    }
}

void Parser::next_token() {
    // Advance the current token index
    cur_idx++;
}

Token Parser::cur_token() {
    // Get the current token
    return tokens[cur_idx];
}

bool Parser::peek_token(TokenType type, int cnt) {
    // Check if the token `cnt` ahead is of the given type
    if (cur_idx + cnt < tokens.size()) {
        return tokens[cur_idx + cnt].type == type;
    } else {
        // If the token index is out of bounds, return false
        return false;
    }
}

bool Parser::check(TokenType type) {
    // Check the current token type
    return type == cur_token().type;
}

bool Parser::accept(TokenType type) {
    // Accept the current token if it is of the given type
    if (check(type)) {
        next_token();
        return true;
    }

    // Otherwise, return false
    return false;
}

void Parser::expect(TokenType type) {
    // Expect the current token to be of the given type
    if (!accept(type)) {
        syntax_error(token_values[type], cur_token());
    }
}

void Parser::accept_new_lines() {
    // Accept as many new line tokens as possible
    while (accept(NEW_LINE))
        ;
}

void Parser::sync() {
    error_manager->handle_error();

    // Skip tokens until a grounding token is found
    while (true) {
        switch (cur_token().type) {
        // Gounding tokens
        case IF_KEYWORD:
        case FOR_KEYWORD:
        case END_OF_FILE:
        case NEW_LINE:
            // Grounding token found
            accept_new_lines();
            return;
        default:
            // Skip the current token
            next_token();
            continue;
        }
    }
}

void Parser::syntax_error(const std::string &expected, const Token &actual) {
    // Report a syntax error at a the token's position
    error_manager->error_at_pos("Expected " + expected + " but got " +
                                    token_values[actual.type],
                                actual.line + 1,
                                actual.column + 1,
                                false);
}
