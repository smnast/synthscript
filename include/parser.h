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
     * @brief Create a parser object.
     * @param program_tokens The tokens of the program.
     * @param error_manager The error manager object.
     *
     * @note
     * The parser does not take ownership of the error manager object.
     */
    Parser(std::vector<Token> program_tokens, ErrorManager *error_manager);

    /**
     * @brief Parses the program tokens into an AST.
     * @param program_tokens The tokens of the program.
     * @return ProgramNode* A pointer to the root node of the AST.
     */
    ProgramNode *parse_program();

private:
    /**
     * @brief The error manager to use for error handling.
     */
    ErrorManager *error_manager;

    /**
     * @brief The tokens to be parsed.
     */
    std::vector<Token> program_tokens;

    /**
     * @brief Current token index.
     */
    int cur_idx;

    /**
     * @brief Stores the tokens of the program currently being parsed.
     */
    std::vector<Token> tokens;

    // Parsing functions
    ASTNode *parse_statement(), *parse_compound_statement();
    ASTNode *parse_array_literal(), *parse_array_subscript();
    ASTNode *parse_cast();
    ASTNode *parse_if_statement(), *parse_while_statement(), *parse_for_statement(),
        *parse_repeat_statement();
    ASTNode *parse_break_statement(), *parse_continue_statement(), *parse_return_statement();
    ASTNode *parse_identifier(), *parse_literal();
    ASTNode *parse_function_declaration(), *parse_call();
    ASTNode *parse_primary_expression(), *parse_assignment_expression(),
        *parse_logical_or_expression(), *parse_logical_and_expression(),
        *parse_bitwise_or_expression(), *parse_bitwise_xor_expression(),
        *parse_bitwise_and_expression(), *parse_equality_expression(),
        *parse_relational_expression(), *parse_range_literal_expression(),
        *parse_additive_expression(), *parse_multiplicative_expression(), *parse_unary_expression(),
        *parse_factor_expression();

    /**
     * @brief Advanced to the next token.
     */
    void next_token();

    /**
     * @brief Get the current token.
     * @return The current token.
     */
    Token cur_token();

    /**
     * @brief Check if the token `cnt` ahead is of the given type, without advancing the token
     * index.
     * @param type The type to check.
     * @param cnt The number of tokens to look ahead.
     * @return True if the token is of the given type, false otherwise.
     */
    bool peek_token(TokenType type, int cnt);

    /**
     * @brief Check if the current token is of the given type.
     * @param type The type to check.
     */
    bool check(TokenType type);

    /**
     * @brief Accept the current token if it is of the given type.
     * @param type The type to accept.
     * @return True if the token was accepted, false otherwise.
     */
    bool accept(TokenType type);

    /**
     * @brief Expect the current token to be of the given type, otherwise throw an error.
     * @param type The type to expect.
     */
    void expect(TokenType type);

    /**
     * @brief Accept as many new line tokens as possible.
     */
    void accept_new_lines();

    /**
     * @brief Skip tokens until a grounding token is found.
     * Used to recover from syntax errors.
     */
    void sync();

    /**
     * @brief Report a syntax error.
     * @param expected The expected token.
     * @param actual The actual token.
     */
    void syntax_error(const std::string &expected, const Token &actual);
};

#endif // SYNTHSCRIPT_PARSER_H
