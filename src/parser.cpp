#include "parser.h"

int Parser::cur_idx = 0;
std::vector<Token> Parser::tokens = std::vector<Token>();

void Parser::next_token() {
    cur_idx++;
}

Token Parser::cur_token() {
    return tokens[cur_idx];
}

bool Parser::peek_token(TokenType type, int cnt) {
    if (cur_idx + cnt < tokens.size()) {
        return tokens[cur_idx + cnt].token_type == type;
    }
    return tokens.back().token_type == type;
}

bool Parser::check(TokenType type) {
    return type == cur_token().token_type;
}

bool Parser::accept(TokenType type) {
    if (check(type)) {
        next_token();
        return true;
    }

    return false;
}

void Parser::expect(TokenType type) {
    if (!accept(type)) {
        parser_error(token_names[type], cur_token());
    }
}

void Parser::accept_new_lines() {
    while (accept(NEW_LINE));
}

void Parser::sync() {
    while (true) {
        switch (cur_token().token_type) {
            case IF_KEYWORD:
            case FOR_KEYWORD:
            case END_OF_FILE:
            case NEW_LINE:
                return;
            default:
                next_token();
                continue;
        }
    }
}

void Parser::parser_error(const std::string &expected, const Token &actual) {
    Error::error_at_pos("Expected " + expected + " but got " + token_names[actual.token_type], actual.line, actual.column, false);
}

ProgramNode *Parser::parse_program(std::vector<Token> program_tokens) {
    tokens = std::move(program_tokens);
    cur_idx = 0;

    std::vector<ASTNode*> statements;
    while (!check(END_OF_FILE)) {
        if (Error::check_error()) {
            Error::handle_error();
            sync();
            continue;
        }
        auto *statement_node = parse_statement();
        if (statement_node == nullptr) break;
        statements.push_back(statement_node);

        accept_new_lines();
    }

    return new ProgramNode(statements, 0, 0);
}

ASTNode *Parser::parse_statement() {
    accept_new_lines();
    ASTNode *node = nullptr;

    if (check(FUNCTION_KEYWORD)) {
        node = parse_function_declaration();
    } else if (check(IF_KEYWORD)) {
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
    } else if (!check(END_OF_FILE) && !check(RBRACE)) {
        node = parse_primary_expression();
    }

    return node;
}

ASTNode *Parser::parse_compound_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(LBRACE);
    std::vector<ASTNode*> statements;
    while (!check(RBRACE) && !check(END_OF_FILE)) {
        if (Error::check_error()) {
            Error::handle_error();
            sync();
            continue;
        }
        auto *statement_node = parse_statement();
        if (statement_node == nullptr) continue;
        statements.push_back(statement_node);

        accept_new_lines();
    }
    expect(RBRACE);
    return new CompoundStatementNode(statements, line, col);
}

ASTNode *Parser::parse_function_declaration() {
    int line = cur_token().line, col = cur_token().column;
    expect(FUNCTION_KEYWORD);
    std::string identifier = cur_token().value;
    expect(IDENTIFIER);
    expect(LPAREN);
    std::vector<std::string> parameters;
    while (!check(RPAREN) && !check(END_OF_FILE)) {
        if (Error::check_error()) return nullptr;
        std::string parameter = cur_token().value;
        expect(IDENTIFIER);
        parameters.push_back(parameter);
        if (accept(COMMA)) continue;
    }
    expect(RPAREN);
    ASTNode *body = parse_compound_statement();

    return new FunctionDeclarationNode(identifier, parameters, body, line, col);
}

ASTNode *Parser::parse_function_statement() {
    int line = cur_token().line, col = cur_token().column;
    std::string identifier = cur_token().value;
    expect(IDENTIFIER);
    expect(LPAREN);
    std::vector<ASTNode*> arguments;
    while (!check(RPAREN) && !check(END_OF_FILE)) {
        if (Error::check_error()) return nullptr;
        auto *argument = parse_primary_expression();
        if (argument == nullptr) continue;
        arguments.push_back(argument);
        if (accept(COMMA)) continue;
    }
    expect(RPAREN);
    return new FunctionStatementNode(identifier, arguments, line, col);
}

ASTNode *Parser::parse_array_literal() {
    int line = cur_token().line, col = cur_token().column;
    expect(LBRACKET);
    std::vector<ASTNode*> values;
    while (!check(RBRACKET) && !check(END_OF_FILE)) {
        if (Error::check_error()) return nullptr;
        auto *value = parse_primary_expression();
        if (value == nullptr) continue;
        values.push_back(value);
        if (accept(COMMA)) continue;
    }
    expect(RBRACKET);
    return new ArrayLiteralNode(values, line, col);
}

ASTNode *Parser::parse_array_subscript() {
    ASTNode *identifier = parse_identifier();
    while (accept(LBRACKET)) {
        if (Error::check_error()) return nullptr;
        int line = cur_token().line, col = cur_token().column;
        auto *index = parse_primary_expression();
        identifier = new SubscriptOpNode(identifier, index, line, col);
        expect(RBRACKET);
    }

    return identifier;
}

ASTNode *Parser::parse_cast() {
    int line = cur_token().line, col = cur_token().column;
    if (token_is_type(cur_token().token_type)) {
        TokenType token_type = cur_token().token_type;
        next_token();
        expect(LPAREN);
        auto *value = parse_primary_expression();
        expect(RPAREN);
        return new CastOpNode(token_to_type(token_type), value, line, col);
    }
    parser_error("type", cur_token());
    return nullptr;
}

ASTNode *Parser::parse_if_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(IF_KEYWORD);
    auto *condition = parse_primary_expression();
    auto *body = parse_compound_statement();
    ASTNode *else_body = nullptr;
    if (accept(ELSE_KEYWORD)) {
        if (check(IF_KEYWORD)) {
            else_body = parse_if_statement();
        } else {
            else_body = parse_compound_statement();
        }
    }
    if (condition == nullptr) return nullptr;
    return new IfStatementNode(condition, body, else_body, line, col);
}

ASTNode *Parser::parse_while_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(WHILE_KEYWORD);
    auto *condition = parse_primary_expression();
    auto *body = parse_compound_statement();
    if (condition == nullptr) return nullptr;
    return new WhileStatementNode(condition, body, line, col);
}

ASTNode *Parser::parse_for_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(FOR_KEYWORD);
    std::string identifier = cur_token().value;
    expect(IDENTIFIER);
    expect(IN_KEYWORD);
    auto *iterable = parse_primary_expression();
    auto *body = parse_compound_statement();
    if (iterable == nullptr) return nullptr;
    return new ForStatementNode(identifier, iterable, body, line, col);
}

ASTNode *Parser::parse_repeat_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(REPEAT_KEYWORD);
    auto *count = parse_primary_expression();
    auto *body = parse_compound_statement();
    if (count == nullptr) return nullptr;
    return new RepeatStatementNode(count, body, line, col);
}

ASTNode *Parser::parse_break_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(BREAK_KEYWORD);
    return new BreakStatementNode(line, col);
}

ASTNode *Parser::parse_continue_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(CONTINUE_KEYWORD);
    return new ContinueStatementNode(line, col);
}

ASTNode *Parser::parse_return_statement() {
    int line = cur_token().line, col = cur_token().column;
    expect(RETURN_KEYWORD);
    ASTNode *value = nullptr;
    if (!check(END_OF_FILE) && !check(NEW_LINE) && !check(RBRACE)) {
        value = parse_primary_expression();
    }
    if (value == nullptr) return nullptr;
    return new ReturnStatementNode(value, line, col);
}

ASTNode *Parser::parse_identifier() {
    int line = cur_token().line, col = cur_token().column;
    std::string identifier = cur_token().value;
    expect(IDENTIFIER);
    return new IdentifierNode(identifier, line, col);
}

ASTNode *Parser::parse_literal() {
    int line = cur_token().line, col = cur_token().column;
    Type type = token_to_type(cur_token().token_type);
    std::string value = cur_token().value;
    next_token();
    return new LiteralNode(type, value, line, col);
}

ASTNode *Parser::parse_primary_expression() {
    return parse_assignment_expression();
}

ASTNode *Parser::parse_assignment_expression() {
    int line = cur_token().line, col = cur_token().column;
    ASTNode *left = parse_logical_or_expression();
    if (accept(ASSIGNMENT_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        auto *right = parse_assignment_expression();
        left = new AssignmentNode(left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_logical_or_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_logical_and_expression();
    while (check(LOGICAL_OR_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(LOGICAL_OR_OPERATOR);
        auto *right = parse_logical_and_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_logical_and_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_bitwise_or_expression();
    while (check(LOGICAL_AND_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(LOGICAL_AND_OPERATOR);
        auto *right = parse_bitwise_or_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_bitwise_or_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_bitwise_xor_expression();
    while (check(BITWISE_OR_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(BITWISE_OR_OPERATOR);
        auto *right = parse_bitwise_xor_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_bitwise_xor_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_bitwise_and_expression();
    while (check(BITWISE_XOR_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(BITWISE_XOR_OPERATOR);
        auto *right = parse_bitwise_and_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_bitwise_and_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_equality_expression();
    while (check(BITWISE_AND_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(BITWISE_AND_OPERATOR);
        auto *right = parse_equality_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_equality_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_relational_expression();
    while (check(EQUAL_OPERATOR) || check(NOT_EQUAL_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(cur_token().token_type);
        auto *right = parse_relational_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_relational_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_range_literal_expression();
    while (check(LESS_THAN_OPERATOR) || check(LESS_THAN_EQUAL_OPERATOR)
           || check(GREATER_THAN_OPERATOR) || check(GREATER_THAN_EQUAL_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(cur_token().token_type);
        auto *right = parse_range_literal_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_range_literal_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_additive_expression();
    if (accept(RANGE_SYMBOL)) {
        if (Error::check_error()) return nullptr;

        auto *right = parse_additive_expression();
        left = new RangeLiteralNode(left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_additive_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_multiplicative_expression();
    while (check(ADDITION_OPERATOR) || check(SUBTRACTION_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(cur_token().token_type);
        auto *right = parse_multiplicative_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_multiplicative_expression() {
    int line = cur_token().line, col = cur_token().column;
    auto *left = parse_unary_expression();
    while (check(MULTIPLICATIVE_OPERATOR) || check(DIVISION_OPERATOR) || check(MOD_OPERATOR)) {
        if (Error::check_error()) return nullptr;

        TokenType op = cur_token().token_type;
        accept(cur_token().token_type);
        auto *right = parse_unary_expression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parse_unary_expression() {
    int line = cur_token().line, col = cur_token().column;
    if (check(LOGICAL_NOT_OPERATOR) || check(BITWISE_NOT_OPERATOR)
        || check(SUBTRACTION_OPERATOR) || check(ADDITION_OPERATOR)) {
        TokenType op = cur_token().token_type;
        accept(cur_token().token_type);
        auto *right = parse_factor_expression();
        return new UnaryOpNode(op, right, line, col);
    } else {
        return parse_factor_expression();
    }
}

ASTNode *Parser::parse_factor_expression() {
    if (token_is_type(cur_token().token_type)) {
        return parse_cast();
    } else if (check(IDENTIFIER) && peek_token(LBRACKET, 1)) {
        return parse_array_subscript();
    } else if (check(IDENTIFIER) && peek_token(LPAREN, 1)) {
        return parse_function_statement();
    } else if (check(IDENTIFIER)) {
        return parse_identifier();
    } else if (check(LPAREN)) {
        expect(LPAREN);
        auto *exp = parse_primary_expression();
        expect(RPAREN);
        return exp;
    } else if (check(LBRACKET)) {
        return parse_array_literal();
    } else if (check(INT_LITERAL) || check(FLOAT_LITERAL) || check(STRING_LITERAL) || check(BOOL_LITERAL)) {
        return parse_literal();
    } else {
        parser_error("factor", cur_token());
    }
    return nullptr;
}

