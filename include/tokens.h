#ifndef SYNTHSCRIPT_TOKENS_H
#define SYNTHSCRIPT_TOKENS_H

#include <string>
#include <vector>

typedef enum {
    ASSIGNMENT_OPERATOR,
    ADDITION_OPERATOR, SUBTRACTION_OPERATOR,
    MULTIPLICATIVE_OPERATOR, DIVISION_OPERATOR, MOD_OPERATOR,
    LOGICAL_AND_OPERATOR, LOGICAL_OR_OPERATOR, LOGICAL_NOT_OPERATOR,
    BITWISE_AND_OPERATOR, BITWISE_OR_OPERATOR, BITWISE_XOR_OPERATOR, BITWISE_NOT_OPERATOR,
    LESS_THAN_OPERATOR, LESS_THAN_EQUAL_OPERATOR, GREATER_THAN_OPERATOR, GREATER_THAN_EQUAL_OPERATOR,
    EQUAL_OPERATOR, NOT_EQUAL_OPERATOR,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,
    COMMA,
    FOR_KEYWORD, REPEAT_KEYWORD, WHILE_KEYWORD,
    IF_KEYWORD, ELSE_KEYWORD,
    CONTINUE_KEYWORD, BREAK_KEYWORD, RETURN_KEYWORD,
    IN_KEYWORD, RANGE_SYMBOL,
    FUNCTION_KEYWORD,
    INT_LITERAL, FLOAT_LITERAL, STRING_LITERAL, BOOL_LITERAL,
    INT_TYPE, FLOAT_TYPE, STRING_TYPE, BOOL_TYPE, VOID_TYPE, ARRAY_TYPE,
    IDENTIFIER, UNDEFINED, ESCAPED_NEW_LINE, NEW_LINE, END_OF_FILE,
} TokenType;

const std::vector<std::string> tokenNames = {
        "'<-'",
        "'+'", "'-'",
        "'*'", "'/'", "'%'",
        "'and'", "'or'", "'not'",
        "'&'", "'|'", "'^'", "'~'",
        "'<'", "'<='", "'>'", "'>='",
        "'='", "'!='",
        "'('", "')'", "'{'", "'}'", "'['", "']'",
        "','",
        "'for'", "'repeat'", "'while'",
        "'if'", "'else'",
        "'next'", "'stop'", "'return'",
        "'in'", "'..'",
        "'function'",
        "int literal", "float literal", "string literal", "bool literal",
        "'int'", "'float'", "'string'", "'bool'", "'void'", "'array'",
        "identifier", "undefined token", "escaped new line", "new line", "end of file",
};

const std::vector<std::pair<TokenType, std::string>> tokenRegexExprs = {
        {ASSIGNMENT_OPERATOR,         R"(\<\-)"},
        {ADDITION_OPERATOR,           R"(\+)"},
        {SUBTRACTION_OPERATOR,        R"(\-)"},
        {MULTIPLICATIVE_OPERATOR,     R"(\*)"},
        {DIVISION_OPERATOR,           R"(\/)"},
        {MOD_OPERATOR,                R"(\%)"},
        {LOGICAL_AND_OPERATOR,        R"(\band\b)"},
        {LOGICAL_OR_OPERATOR,         R"(\bor\b)"},
        {LOGICAL_NOT_OPERATOR,        R"(\bnot\b)"},
        {BITWISE_AND_OPERATOR,        R"(\&)"},
        {BITWISE_OR_OPERATOR,         R"(\|)"},
        {BITWISE_XOR_OPERATOR,         R"(\^)"},
        {BITWISE_NOT_OPERATOR,        R"(\~)"},
        {LESS_THAN_OPERATOR,          R"(<)"},
        {LESS_THAN_EQUAL_OPERATOR,    R"(<=)"},
        {GREATER_THAN_OPERATOR,       R"(>)"},
        {GREATER_THAN_EQUAL_OPERATOR, R"(>=)"},
        {EQUAL_OPERATOR,              R"(=)"},
        {NOT_EQUAL_OPERATOR,          R"(!=)"},
        {LPAREN,                      R"(\()"},
        {RPAREN,                      R"(\))"},
        {LBRACE,                      R"(\{)"},
        {RBRACE,                      R"(\})"},
        {LBRACKET,                    R"(\[)"},
        {RBRACKET,                    R"(\])"},
        {COMMA,                       R"(\,)"},
        {FOR_KEYWORD,                 R"(\bfor\b)"},
        {REPEAT_KEYWORD,              R"(\brepeat\b)"},
        {WHILE_KEYWORD,               R"(\bwhile\b)"},
        {IF_KEYWORD,                  R"(\bif\b)"},
        {ELSE_KEYWORD,                R"(\belse\b)"},
        {CONTINUE_KEYWORD,            R"(\bnext\b)"},
        {BREAK_KEYWORD,               R"(\bstop\b)"},
        {RETURN_KEYWORD,              R"(\breturn\b)"},
        {IN_KEYWORD,                  R"(\bin\b)"},
        {RANGE_SYMBOL,                R"(\.\.)"},
        {FUNCTION_KEYWORD,            R"(\bfunction\b)"},
        {FLOAT_LITERAL,               R"(\d+\.\d+)"},
        {INT_LITERAL,                 R"(\d+)"},
        {STRING_LITERAL,              R"("(?:[^"\\]|\\.)*")"},
        {BOOL_LITERAL,                R"(\b(?:true|false)\b)"},
        {INT_TYPE,                    R"(\bint\b)"},
        {FLOAT_TYPE,                  R"(\bfloat\b)"},
        {STRING_TYPE,                 R"(\bstring\b)"},
        {BOOL_TYPE,                   R"(\bbool\b)"},
        {VOID_TYPE,                   R"(\bvoid\b)"},
        {ARRAY_TYPE,                  R"(\barray\b)"},
        {IDENTIFIER,                  R"([a-zA-Z_]\w*)"},
        {ESCAPED_NEW_LINE,            R"(\\ *\n)"},
        {NEW_LINE,                    R"(\n)"},
        {UNDEFINED,                   R"([^\ \n]+)"},
};

struct Token {
    const TokenType tokenType{};
    const std::string value{};
    const int lineNumber, columnNumber;

    Token(TokenType type, std::string val, int lineNumber, int columnNumber) : tokenType(type), value(std::move(val)), lineNumber(lineNumber), columnNumber(columnNumber) {}
};

static bool token_is_type(TokenType type) {
    return type == INT_TYPE || type == FLOAT_TYPE || type == BOOL_TYPE || type == STRING_TYPE || type == VOID_TYPE || type == ARRAY_TYPE;
}

static bool token_is_literal(TokenType type) {
    return type == INT_LITERAL || type == FLOAT_LITERAL || type == BOOL_LITERAL || type == STRING_LITERAL;
}

#endif //SYNTHSCRIPT_TOKENS_H
