#ifndef SYNTHSCRIPT_PARSER_H
#define SYNTHSCRIPT_PARSER_H

#include "AST/AST_node.h"
#include "AST/AST_nodes.h"
#include "error.h"
#include "tokens.h"
#include <vector>

class Parser {
public:
    static ProgramNode *parse_program(std::vector<Token> program_tokens);

private:
    static int cur_idx;
    static std::vector<Token> tokens;

    static void next_token();
    static Token cur_token();
    static bool peek_token(TokenType type, int cnt);

    static bool check(TokenType type);
    static bool accept(TokenType type);
    static void expect(TokenType type);

    static void accept_new_lines();
    static void sync();

    static void parser_error(const std::string &expected, const Token &actual);

    static ASTNode *parse_statement(), *parse_compound_statement();
    static ASTNode *parse_function_declaration(), *parse_function_statement();
    static ASTNode *parse_array_literal(), *parse_array_subscript();
    static ASTNode *parse_cast();
    static ASTNode *parse_if_statement(), *parse_while_statement(), *parse_for_statement(), *parse_repeat_statement();
    static ASTNode *parse_break_statement(), *parse_continue_statement(), *parse_return_statement();
    static ASTNode *parse_identifier(), *parse_literal();
    static ASTNode *parse_primary_expression(), *parse_assignment_expression(), *parse_logical_or_expression(), *parse_logical_and_expression(), *parse_bitwise_or_expression(), *parse_bitwise_xor_expression(), *parse_bitwise_and_expression(), *parse_equality_expression(), *parse_relational_expression(), *parse_range_literal_expression(), *parse_additive_expression(), *parse_multiplicative_expression(), *parse_unary_expression(), *parse_factor_expression();
};

#endif // SYNTHSCRIPT_PARSER_H
