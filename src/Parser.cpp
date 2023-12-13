#include "Parser.h"

int Parser::curIdx = 0;
std::vector<Token> Parser::tokens = std::vector<Token>();

void Parser::nextToken() {
    curIdx++;
}

Token Parser::curToken() {
    return tokens[curIdx];
}

bool Parser::peekToken(TokenType type, int cnt) {
    if (curIdx + cnt < tokens.size()) {
        return tokens[curIdx + cnt].tokenType == type;
    }
    return tokens.back().tokenType == type;
}

bool Parser::check(TokenType type) {
    return type == curToken().tokenType;
}

bool Parser::accept(TokenType type) {
    if (check(type)) {
        nextToken();
        return true;
    }

    return false;
}

void Parser::expect(TokenType type) {
    if (!accept(type)) {
        parserError("token " + tokenNames[type], curToken());
    }
}

void Parser::sync() {
    while (true) {
        switch (curToken().tokenType) {
            case IF_KEYWORD:
            case FOR_KEYWORD:
            case END_OF_FILE:
            case RBRACE:
            case RPAREN:
                return;
            default:
                nextToken();
                continue;
        }
    }
}

void Parser::parserError(const std::string &expected, const Token &actual) {
    Error::posError("Expected " + expected + " but got " + tokenNames[actual.tokenType], actual.lineNumber, actual.columnNumber);
}

