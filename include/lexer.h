#ifndef SYNTHSCRIPT_LEXER_H
#define SYNTHSCRIPT_LEXER_H

#include "tokens.h"

/**
 * @class Lexer
 * @brief Provides functions for lexical analysis of the code.
 */
class Lexer {
public:
    /**
     * @brief Parse the input code into tokens.
     * @param code The code to be parsed.
     * @return std::vector<Token>
     */
    static std::vector<Token> parse_tokens(std::string &code);

private:
    /**
     * @brief Stores the line of each character in the code.
     */
    static std::vector<int> line_prefix;

    /**
     * @brief Stores the beginning offset of the line for each character in the code.
     */
    static std::vector<int> line_offset_prefix;

    /**
     * @brief Combines all the regexes into one expression.
     * @return The full regex expression.
     */
    static std::string combine_regex();

    /**
     * @brief Prepare the line and column prefixes.
     * @param code The code to be parsed.
     */
    static void prepare_prefixes(std::string &code);

    /**
     * @brief Parse the code into tokens (lexical analysis).
     * @param code The code to be parsed.
     * @param token_regex The combined regex for all the tokens.
     */
    static std::vector<Token> get_tokens(std::string &code, std::string &token_regex);

    /**
     * @brief Get the line of a character in the code.
     * @param position The position of the character.
     * @return The line of the character.
     */
    static int get_line(int position);

    /**
     * @brief Get the column of a character in the code.
     * @param position The position of the character.
     * @return The column of the character, relative to the line.
     */
    static int get_column(int position);

    /**
     * @brief Report an error for an undefined token.
     * @param token The token that is undefined.
     * @param line The line of the token.
     * @param column The column of the token.
     */
    static void lexer_error(const std::string &token, int line, int column);
};

#endif // SYNTHSCRIPT_LEXER_H
