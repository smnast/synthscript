#ifndef SYNTHSCRIPT_LEXER_H
#define SYNTHSCRIPT_LEXER_H

#include "error_manager.h"
#include "tokens.h"
#include <utility>

/**
 * @class Lexer
 * @brief Provides functions for lexical analysis of the code.
 */
class Lexer {
public:
    /**
     * @brief Construct a new Lexer object.
     * @param code The code to be parsed into tokens.
     * @param error_manager The error manager to use for error handling.
     *
     * @note
     * The lexer does not take ownership of the error manager.
     */
    Lexer(const std::string &code, ErrorManager *error_manager);

    /**
     * @brief Parse the input code into tokens.
     * @param code The code to be parsed.
     * @return std::vector<Token>
     */
    std::vector<Token> parse_tokens();

private:
    /**
     * @brief The error manager to use for error handling.
     */
    ErrorManager *error_manager;

    /**
     * @brief The code to be parsed into tokens.
     */
    std::string code;

    /**
     * @brief Stores the line of each character in the code.
     */
    std::vector<int> line_prefix;

    /**
     * @brief Stores the beginning offset of the line for each character in the code.
     */
    std::vector<int> line_offset_prefix;

    /**
     * @brief Combines all the regexes into one expression.
     * @return The full regex expression.
     */
    std::string combine_regex();

    /**
     * @brief Prepare the line and column prefixes.
     * @param code The code to be parsed.
     */
    void prepare_prefixes(std::string &code);

    /**
     * @brief Parse the code into tokens (lexical analysis).
     * @param code The code to be parsed.
     * @param token_regex The combined regex for all the tokens.
     */
    std::vector<Token> get_tokens(std::string &code, std::string &token_regex);

    /**
     * @brief Get the line of a character in the code.
     * @param position The position of the character.
     * @return The line of the character.
     */
    int get_line(size_t position);

    /**
     * @brief Get the column of a character in the code.
     * @param position The position of the character.
     * @return The column of the character, relative to the line.
     */
    int get_column(size_t position);

    /**
     * @brief Report an error for an undefined token.
     * @param token The token that is undefined.
     * @param line The line of the token.
     * @param column The column of the token.
     */
    void lexer_error(const std::string &token, int line, int column);
};

#endif // SYNTHSCRIPT_LEXER_H
