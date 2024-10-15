#ifndef SYNTHSCRIPT_LEXER_H
#define SYNTHSCRIPT_LEXER_H

#include "error.h"
#include "tokens.h"
#include <regex>

class Lexer {
public:
    static std::vector<Token> parse_tokens(std::string &file);

private:
    static std::vector<int> linePrefix, columnPrefix;
    static std::string combine_regex();
    static void prepare_line_prefix(std::string &file);
    static void prepare_column_prefix(std::string &file);
    static int get_line(int position);
    static int get_column(int position);
    static void lexer_error(const std::string &token, int line, int column);
};

#endif //SYNTHSCRIPT_LEXER_H
