#ifndef SYNTHSCRIPT_LEXER_H
#define SYNTHSCRIPT_LEXER_H

#include "error.h"
#include "tokens.h"
#include <regex>

class Lexer {
public:
    static std::vector<Token> parseTokens(std::string &file);
private:
    static std::vector<int> linePrefix, columnPrefix;
    static std::string combineRegex();
    static void prepareLinePrefix(std::string &file);
    static void prepareColumnPrefix(std::string &file);
    static int getLineNumber(int position);
    static int getColumnNumber(int position);
    static void lexerError(const std::string &token, int line, int column);
};

#endif //SYNTHSCRIPT_LEXER_H
