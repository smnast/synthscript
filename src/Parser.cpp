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
        parserError(tokenNames[type], curToken());
    }
}

void Parser::acceptNewlines() {
    while (accept(NEW_LINE));
}

void Parser::sync() {
    while (true) {
        switch (curToken().tokenType) {
            case IF_KEYWORD:
            case FOR_KEYWORD:
            case END_OF_FILE:
            case NEW_LINE:
                return;
            default:
                nextToken();
                continue;
        }
    }
}

void Parser::parserError(const std::string &expected, const Token &actual) {
    Error::posError("Expected " + expected + " but got " + tokenNames[actual.tokenType], actual.lineNumber, actual.columnNumber, false);
}

ProgramNode *Parser::parseProgram(std::vector<Token> programTokens) {
    tokens = std::move(programTokens);
    curIdx = 0;

    std::vector<ASTNode*> statements;
    while (!check(END_OF_FILE)) {
        if (Error::checkError()) {
            Error::handleError();
            sync();
            continue;
        }
        auto *statementNode = parseStatement();
        if (statementNode == nullptr) break;
        statements.push_back(statementNode);

        acceptNewlines();
    }

    return new ProgramNode(statements, 0, 0);
}

ASTNode *Parser::parseStatement() {
    acceptNewlines();
    ASTNode *node = nullptr;

    if (check(FUNCTION_KEYWORD)) {
        node = parseFunctionDeclaration();
    } else if (check(IF_KEYWORD)) {
        node = parseIfStatement();
    } else if (check(WHILE_KEYWORD)) {
        node = parseWhileStatement();
    } else if (check(FOR_KEYWORD)) {
        node = parseForStatement();
    } else if (check(BREAK_KEYWORD)) {
        node = parseBreakStatement();
    } else if (check(CONTINUE_KEYWORD)) {
        node = parseContinueStatement();
    } else if (check(RETURN_KEYWORD)) {
        node = parseReturnStatement();
    } else if (!check(END_OF_FILE) && !check(RBRACE)) {
        node = parsePrimaryExpression();
    }

    return node;
}

ASTNode *Parser::parseCompoundStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(LBRACE);
    std::vector<ASTNode*> statements;
    while (!check(RBRACE) && !check(END_OF_FILE)) {
        if (Error::checkError()) {
            Error::handleError();
            sync();
            continue;
        }
        auto *statementNode = parseStatement();
        if (statementNode == nullptr) continue;
        statements.push_back(statementNode);

        acceptNewlines();
    }
    expect(RBRACE);
    return new CompoundStatementNode(statements, line, col);
}

ASTNode *Parser::parseFunctionDeclaration() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(FUNCTION_KEYWORD);
    std::string identifier = curToken().value;
    expect(IDENTIFIER);
    expect(LPAREN);
    std::vector<std::string> parameters;
    while (!check(RPAREN) && !check(END_OF_FILE)) {
        if (Error::checkError()) return nullptr;
        std::string parameter = curToken().value;
        expect(IDENTIFIER);
        parameters.push_back(parameter);
        if (accept(COMMA)) continue;
    }
    expect(RPAREN);
    ASTNode *body = parseCompoundStatement();

    return new FunctionDeclarationNode(identifier, parameters, body, line, col);
}

ASTNode *Parser::parseFunctionStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    std::string identifier = curToken().value;
    expect(IDENTIFIER);
    expect(LPAREN);
    std::vector<ASTNode*> arguments;
    while (!check(RPAREN) && !check(END_OF_FILE)) {
        if (Error::checkError()) return nullptr;
        auto *argument = parsePrimaryExpression();
        if (argument == nullptr) continue;
        arguments.push_back(argument);
        if (accept(COMMA)) continue;
    }
    expect(RPAREN);
    return new FunctionStatementNode(identifier, arguments, line, col);
}

ASTNode *Parser::parseArrayLiteral() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(LBRACKET);
    std::vector<ASTNode*> values;
    while (!check(RBRACKET) && !check(END_OF_FILE)) {
        if (Error::checkError()) return nullptr;
        auto *value = parsePrimaryExpression();
        if (value == nullptr) continue;
        values.push_back(value);
        if (accept(COMMA)) continue;
    }
    expect(RBRACKET);
    return new ArrayLiteralNode(values, line, col);
}

ASTNode *Parser::parseArraySubscript() {
    ASTNode *identifier = parseIdentifier();
    while (accept(LBRACKET)) {
        if (Error::checkError()) return nullptr;
        int line = curToken().lineNumber, col = curToken().columnNumber;
        auto *index = parsePrimaryExpression();
        identifier = new SubscriptOpNode(identifier, index, line, col);
        expect(RBRACKET);
    }

    return identifier;
}

ASTNode *Parser::parseCast() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    if (tokenIsType(curToken().tokenType)) {
        TokenType tokenType = curToken().tokenType;
        nextToken();
        expect(LPAREN);
        auto *value = parsePrimaryExpression();
        expect(RPAREN);
        return new CastOpNode(tokenToType(tokenType), value, line, col);
    }
    parserError("type", curToken());
    return nullptr;
}

ASTNode *Parser::parseIfStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(IF_KEYWORD);
    expect(LPAREN);
    auto *condition = parsePrimaryExpression();
    expect(RPAREN);
    auto *body = parseCompoundStatement();
    ASTNode *elseBody = nullptr;
    if (accept(ELSE_KEYWORD)) {
        elseBody = parseCompoundStatement();
    }
    if (condition == nullptr) return nullptr;
    return new IfStatementNode(condition, body, elseBody, line, col);
}

ASTNode *Parser::parseWhileStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(WHILE_KEYWORD);
    expect(LPAREN);
    auto *condition = parsePrimaryExpression();
    expect(RPAREN);
    auto *body = parseCompoundStatement();
    if (condition == nullptr) return nullptr;
    return new WhileStatementNode(condition, body, line, col);
}

ASTNode *Parser::parseForStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(FOR_KEYWORD);
    expect(LPAREN);
    std::string identifier = curToken().value;
    expect(IDENTIFIER);
    expect(IN_KEYWORD);
    auto *start = parsePrimaryExpression();
    expect(RANGE_SYMBOL);
    auto *end = parsePrimaryExpression();
    expect(RPAREN);
    auto *body = parseCompoundStatement();
    if (start == nullptr || end == nullptr) return nullptr;
    return new ForStatementNode(identifier, start, end, body, line, col);
}

ASTNode *Parser::parseBreakStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(BREAK_KEYWORD);
    return new BreakStatementNode(line, col);
}

ASTNode *Parser::parseContinueStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(CONTINUE_KEYWORD);
    return new ContinueStatementNode(line, col);
}

ASTNode *Parser::parseReturnStatement() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    expect(RETURN_KEYWORD);
    ASTNode *value = nullptr;
    if (!check(END_OF_FILE) && !check(NEW_LINE) && !check(RBRACE)) {
        value = parsePrimaryExpression();
    }
    if (value == nullptr) return nullptr;
    return new ReturnStatementNode(value, line, col);
}

ASTNode *Parser::parseIdentifier() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    std::string identifier = curToken().value;
    expect(IDENTIFIER);
    return new IdentifierNode(identifier, line, col);
}

ASTNode *Parser::parseLiteral() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    Type type = tokenToType(curToken().tokenType);
    std::string value = curToken().value;
    nextToken();
    return new LiteralNode(type, value, line, col);
}

ASTNode *Parser::parsePrimaryExpression() {
    return parseAssignmentExpression();
}

ASTNode *Parser::parseAssignmentExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    ASTNode *left = parseLogicalOrExpression();
    if (accept(ASSIGNMENT_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        auto *right = parseAssignmentExpression();
        left = new AssignmentNode(left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseLogicalOrExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseLogicalAndExpression();
    while (check(LOGICAL_OR_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(LOGICAL_OR_OPERATOR);
        auto *right = parseLogicalAndExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseLogicalAndExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseBitwiseOrExpression();
    while (check(LOGICAL_AND_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(LOGICAL_AND_OPERATOR);
        auto *right = parseBitwiseOrExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseBitwiseOrExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseBitwiseXorExpression();
    while (check(BITWISE_OR_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(BITWISE_OR_OPERATOR);
        auto *right = parseBitwiseXorExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseBitwiseXorExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseBitwiseAndExpression();
    while (check(BITWISE_XOR_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(BITWISE_XOR_OPERATOR);
        auto *right = parseBitwiseAndExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseBitwiseAndExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseEqualityExpression();
    while (check(BITWISE_AND_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(BITWISE_AND_OPERATOR);
        auto *right = parseEqualityExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseEqualityExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseRelationalExpression();
    while (check(EQUAL_OPERATOR) || check(NOT_EQUAL_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(curToken().tokenType);
        auto *right = parseRelationalExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseRelationalExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseAdditiveExpression();
    while (check(LESS_THAN_OPERATOR) || check(LESS_THAN_EQUAL_OPERATOR)
           || check(GREATER_THAN_OPERATOR) || check(GREATER_THAN_EQUAL_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(curToken().tokenType);
        auto *right = parseAdditiveExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseAdditiveExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseMultiplicativeExpression();
    while (check(ADDITION_OPERATOR) || check(SUBTRACTION_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(curToken().tokenType);
        auto *right = parseMultiplicativeExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseMultiplicativeExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    auto *left = parseUnaryExpression();
    while (check(MULTIPLICATIVE_OPERATOR) || check(DIVISION_OPERATOR) || check(MOD_OPERATOR)) {
        if (Error::checkError()) return nullptr;

        TokenType op = curToken().tokenType;
        accept(curToken().tokenType);
        auto *right = parseUnaryExpression();
        left = new BinOpNode(op, left, right, line, col);
    }
    return left;
}

ASTNode *Parser::parseUnaryExpression() {
    int line = curToken().lineNumber, col = curToken().columnNumber;
    if (check(LOGICAL_NOT_OPERATOR) || check(BITWISE_NOT_OPERATOR)
        || check(SUBTRACTION_OPERATOR) || check(ADDITION_OPERATOR)) {
        TokenType op = curToken().tokenType;
        accept(curToken().tokenType);
        auto *right = parseFactor();
        return new UnaryOpNode(op, right, line, col);
    } else {
        return parseFactor();
    }
}

ASTNode *Parser::parseFactor() {
    if (tokenIsType(curToken().tokenType)) {
        return parseCast();
    } else if (check(IDENTIFIER) && peekToken(LBRACKET, 1)) {
        return parseArraySubscript();
    } else if (check(IDENTIFIER) && peekToken(LPAREN, 1)) {
        return parseFunctionStatement();
    } else if (check(IDENTIFIER)) {
        return parseIdentifier();
    } else if (check(LPAREN)) {
        expect(LPAREN);
        auto *exp = parsePrimaryExpression();
        expect(RPAREN);
        return exp;
    } else if (check(LBRACKET)) {
        return parseArrayLiteral();
    } else if (check(INT_LITERAL) || check(FLOAT_LITERAL) || check(STRING_LITERAL) || check(BOOL_LITERAL)) {
        return parseLiteral();
    } else {
        parserError("factor", curToken());
    }
    return nullptr;
}

