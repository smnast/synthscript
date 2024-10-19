#ifndef SYNTHSCRIPT_PARSER_H
#define SYNTHSCRIPT_PARSER_H

#include "AST/AST_node.h"
#include "AST/AST_nodes.h"
#include "tokens.h"
#include <vector>

/**
 * @class Parser
 * @brief Provides functions for parsing the tokens into an AST.
 */
class Parser {
public:
    /**
     * @brief Parses the program tokens into an AST.
     * @param program_tokens The tokens of the program.
     * @return ProgramNode* A pointer to the root node of the AST.
     */
    static ProgramNode *parse_program(std::vector<Token> program_tokens);

private:
    /**
     * @brief Current token index.
     */
    static int cur_idx;

    /**
     * @brief Stores the tokens of the program currently being parsed.
     */
    static std::vector<Token> tokens;

    // Parsing functions
    static ASTNode *parse_statement(), *parse_compound_statement();
    static ASTNode *parse_function_declaration_statement(), *parse_function_statement();
    static ASTNode *parse_array_literal(), *parse_array_subscript();
    static ASTNode *parse_cast();
    static ASTNode *parse_if_statement(), *parse_while_statement(), *parse_for_statement(),
        *parse_repeat_statement();
    static ASTNode *parse_break_statement(), *parse_continue_statement(), *parse_return_statement();
    static ASTNode *parse_identifier(), *parse_literal();
    static ASTNode *parse_primary_expression(), *parse_assignment_expression(),
        *parse_logical_or_expression(), *parse_logical_and_expression(),
        *parse_bitwise_or_expression(), *parse_bitwise_xor_expression(),
        *parse_bitwise_and_expression(), *parse_equality_expression(),
        *parse_relational_expression(), *parse_range_literal_expression(),
        *parse_additive_expression(), *parse_multiplicative_expression(), *parse_unary_expression(),
        *parse_factor_expression();

    /**
     * @brief Advanced to the next token.
     */
    static void next_token();

    /**
     * @brief Get the current token.
     * @return The current token.
     */
    static Token cur_token();

    /**
     * @brief Check if the token `cnt` ahead is of the given type, without advancing the token
     * index.
     * @param type The type to check.
     * @param cnt The number of tokens to look ahead.
     * @return True if the token is of the given type, false otherwise.
     */
    static bool peek_token(TokenType type, int cnt);

    /**
     * @brief Check if the current token is of the given type.
     * @param type The type to check.
     */
    static bool check(TokenType type);

    /**
     * @brief Accept the current token if it is of the given type.
     * @param type The type to accept.
     * @return True if the token was accepted, false otherwise.
     */
    static bool accept(TokenType type);

    /**
     * @brief Expect the current token to be of the given type, otherwise throw an error.
     * @param type The type to expect.
     */
    static void expect(TokenType type);

    /**
     * @brief Accept as many new line tokens as possible.
     */
    static void accept_new_lines();

    /**
     * @brief Skip tokens until a grounding token is found.
     * Used to recover from syntax errors.
     */
    static void sync();

    /**
     * @brief Report a syntax error.
     * @param expected The expected token.
     * @param actual The actual token.
     */
    static void syntax_error(const std::string &expected, const Token &actual);
};

#endif // SYNTHSCRIPT_PARSER_H
