#ifndef SYNTHSCRIPT_PARSER_H
#define SYNTHSCRIPT_PARSER_H

#include <vector>
#include "AST/ASTNode.h"
#include "AST/ASTNodes.h"
#include "Error.h"
#include "Tokens.h"

class Parser {
public:
    static ProgramNode *parseProgram(std::vector<Token> programTokens);
private:
    static int curIdx;
    static std::vector<Token> tokens;
    static void nextToken();
    static Token curToken();
    static bool peekToken(TokenType type, int cnt);
    static bool check(TokenType type);
    static bool accept(TokenType type);
    static void expect(TokenType type);
    static void acceptNewlines();
    static void sync();
    static void parserError(const std::string &expected, const Token &actual);
    static ASTNode *parseStatement(), *parseCompoundStatement();
    static ASTNode *parseFunctionDeclaration(), *parseFunctionStatement();
    static ASTNode *parseArrayLiteral(), *parseArraySubscript();
    static ASTNode *parseCast();
    static ASTNode *parseIfStatement(), *parseWhileStatement(), *parseForStatement(), *parseRepeatStatement();
    static ASTNode *parseBreakStatement(), *parseContinueStatement(), *parseReturnStatement();
    static ASTNode *parseIdentifier(), *parseLiteral();
    static ASTNode *parsePrimaryExpression(), *parseAssignmentExpression(), *parseLogicalOrExpression(),
            *parseLogicalAndExpression(), *parseBitwiseOrExpression(), *parseBitwiseXorExpression(),
            *parseBitwiseAndExpression(), *parseEqualityExpression(), *parseRelationalExpression(),
            *parseRangeLiteralExpression(), *parseAdditiveExpression(), *parseMultiplicativeExpression(),
            *parseUnaryExpression(), *parseFactor();
};

#endif //SYNTHSCRIPT_PARSER_H
