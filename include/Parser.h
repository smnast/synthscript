#ifndef SYNTHSCRIPT_PARSER_H
#define SYNTHSCRIPT_PARSER_H

#include <vector>
#include "Error.h"
#include "Tokens.h"

class Parser {
    static int curIdx;
    static std::vector<Token> tokens;

    static void nextToken();
    static Token curToken();
    static bool peekToken(TokenType type, int cnt);
    static bool check(TokenType type);
    static bool accept(TokenType type);
    static void expect(TokenType type);
    static void sync();
    static void parserError(const std::string &expected, const Token &actual);
};

#endif //SYNTHSCRIPT_PARSER_H
