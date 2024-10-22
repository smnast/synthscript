#include "lexer.h"
#include "tokens.h"
#include "utils/cout_redirect.h"
#include <doctest/doctest.h>

TEST_CASE("Lexer simple arithmetic") {
    ErrorManager error_manager;
    Lexer lexer("1 + 2", &error_manager);
    std::vector<Token> tokens = lexer.parse_tokens();

    REQUIRE_EQ(tokens.size(), 4);
    CHECK_EQ(tokens[0], Token(TokenType::INT_LITERAL, "1", 1, 1));
    CHECK_EQ(tokens[1], Token(TokenType::ADDITION_OPERATOR, "+", 1, 3));
    CHECK_EQ(tokens[2], Token(TokenType::INT_LITERAL, "2", 1, 5));
    CHECK_EQ(tokens[3], Token(TokenType::END_OF_FILE, "", 1, 6));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer complex arithmetic") {
    ErrorManager error_manager;
    Lexer lexer("(1 + 2 * 3 / (4 - 2) + 17291238 - 00001) % 2", &error_manager);
    std::vector<Token> tokens = lexer.parse_tokens();

    REQUIRE_EQ(tokens.size(), 20);
    CHECK_EQ(tokens[0], Token(TokenType::LPAREN, "(", 1, 1));
    CHECK_EQ(tokens[1], Token(TokenType::INT_LITERAL, "1", 1, 2));
    CHECK_EQ(tokens[2], Token(TokenType::ADDITION_OPERATOR, "+", 1, 4));
    CHECK_EQ(tokens[3], Token(TokenType::INT_LITERAL, "2", 1, 6));
    CHECK_EQ(tokens[4], Token(TokenType::MULTIPLICATIVE_OPERATOR, "*", 1, 8));
    CHECK_EQ(tokens[5], Token(TokenType::INT_LITERAL, "3", 1, 10));
    CHECK_EQ(tokens[6], Token(TokenType::DIVISION_OPERATOR, "/", 1, 12));
    CHECK_EQ(tokens[7], Token(TokenType::LPAREN, "(", 1, 14));
    CHECK_EQ(tokens[8], Token(TokenType::INT_LITERAL, "4", 1, 15));
    CHECK_EQ(tokens[9], Token(TokenType::SUBTRACTION_OPERATOR, "-", 1, 17));
    CHECK_EQ(tokens[10], Token(TokenType::INT_LITERAL, "2", 1, 19));
    CHECK_EQ(tokens[11], Token(TokenType::RPAREN, ")", 1, 20));
    CHECK_EQ(tokens[12], Token(TokenType::ADDITION_OPERATOR, "+", 1, 22));
    CHECK_EQ(tokens[13], Token(TokenType::INT_LITERAL, "17291238", 1, 31));
    CHECK_EQ(tokens[14], Token(TokenType::SUBTRACTION_OPERATOR, "-", 1, 33));
    CHECK_EQ(tokens[15], Token(TokenType::INT_LITERAL, "00001", 1, 39));
    CHECK_EQ(tokens[16], Token(TokenType::RPAREN, ")", 1, 40));
    CHECK_EQ(tokens[17], Token(TokenType::MOD_OPERATOR, "%", 1, 42));
    CHECK_EQ(tokens[18], Token(TokenType::INT_LITERAL, "2", 1, 44));
    CHECK_EQ(tokens[19], Token(TokenType::END_OF_FILE, "", 1, 45));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer with whitespace") {
    ErrorManager error_manager;
    Lexer lexer("  42   +   17  ", &error_manager);
    std::vector<Token> tokens = lexer.parse_tokens();

    REQUIRE_EQ(tokens.size(), 4);
    CHECK_EQ(tokens[0], Token(TokenType::INT_LITERAL, "42", 1, 4));
    CHECK_EQ(tokens[1], Token(TokenType::ADDITION_OPERATOR, "+", 1, 8));
    CHECK_EQ(tokens[2], Token(TokenType::INT_LITERAL, "17", 1, 13));
    CHECK_EQ(tokens[3], Token(TokenType::END_OF_FILE, "", 1, 16));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer with invalid tokens") {
    ErrorManager error_manager;
    CoutRedirect cout_redirect;
    Lexer lexer("42 + @", &error_manager);

    std::vector<Token> tokens;
    cout_redirect.run([&]() { tokens = lexer.parse_tokens(); });

    REQUIRE_EQ(tokens.size(), 4);
    CHECK_EQ(tokens[0], Token(TokenType::INT_LITERAL, "42", 1, 2));
    CHECK_EQ(tokens[1], Token(TokenType::ADDITION_OPERATOR, "+", 1, 4));
    CHECK_EQ(tokens[2], Token(TokenType::UNDEFINED, "@", 1, 6));
    CHECK_EQ(tokens[3], Token(TokenType::END_OF_FILE, "", 1, 7));

    // Error should be reported for '@'
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(cout_redirect.get_string(), "Error: Undefined token: '@' (line 1, column 6)\n");
}

TEST_CASE("Lexer with literals") {
    ErrorManager error_manager;
    Lexer lexer("false true \"string\" \"bool\" \"<-\" 100 001 1.00 0.001 123.456", &error_manager);

    // Check all the literals
    std::vector<Token> tokens = lexer.parse_tokens();
    REQUIRE_EQ(tokens.size(), 11);
    CHECK_EQ(tokens[0], Token(TokenType::BOOL_LITERAL, "false", 1, 5));
    CHECK_EQ(tokens[1], Token(TokenType::BOOL_LITERAL, "true", 1, 10));
    CHECK_EQ(tokens[2], Token(TokenType::STRING_LITERAL, "\"string\"", 1, 19));
    CHECK_EQ(tokens[3], Token(TokenType::STRING_LITERAL, "\"bool\"", 1, 26));
    CHECK_EQ(tokens[4], Token(TokenType::STRING_LITERAL, "\"<-\"", 1, 31));
    CHECK_EQ(tokens[5], Token(TokenType::INT_LITERAL, "100", 1, 35));
    CHECK_EQ(tokens[6], Token(TokenType::INT_LITERAL, "001", 1, 39));
    CHECK_EQ(tokens[7], Token(TokenType::FLOAT_LITERAL, "1.00", 1, 44));
    CHECK_EQ(tokens[8], Token(TokenType::FLOAT_LITERAL, "0.001", 1, 50));
    CHECK_EQ(tokens[9], Token(TokenType::FLOAT_LITERAL, "123.456", 1, 58));
    CHECK_EQ(tokens[10], Token(TokenType::END_OF_FILE, "", 1, 59));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer all operators") {
    ErrorManager error_manager;
    Lexer lexer("<- + - * / % and or not & | ^ ~ < <= > >= = !=", &error_manager);

    // Check all the operators
    std::vector<Token> tokens = lexer.parse_tokens();
    REQUIRE_EQ(tokens.size(), 20);
    CHECK_EQ(tokens[0], Token(TokenType::ASSIGNMENT_OPERATOR, "<-", 1, 2));
    CHECK_EQ(tokens[1], Token(TokenType::ADDITION_OPERATOR, "+", 1, 4));
    CHECK_EQ(tokens[2], Token(TokenType::SUBTRACTION_OPERATOR, "-", 1, 6));
    CHECK_EQ(tokens[3], Token(TokenType::MULTIPLICATIVE_OPERATOR, "*", 1, 8));
    CHECK_EQ(tokens[4], Token(TokenType::DIVISION_OPERATOR, "/", 1, 10));
    CHECK_EQ(tokens[5], Token(TokenType::MOD_OPERATOR, "%", 1, 12));
    CHECK_EQ(tokens[6], Token(TokenType::LOGICAL_AND_OPERATOR, "and", 1, 16));
    CHECK_EQ(tokens[7], Token(TokenType::LOGICAL_OR_OPERATOR, "or", 1, 19));
    CHECK_EQ(tokens[8], Token(TokenType::LOGICAL_NOT_OPERATOR, "not", 1, 23));
    CHECK_EQ(tokens[9], Token(TokenType::BITWISE_AND_OPERATOR, "&", 1, 25));
    CHECK_EQ(tokens[10], Token(TokenType::BITWISE_OR_OPERATOR, "|", 1, 27));
    CHECK_EQ(tokens[11], Token(TokenType::BITWISE_XOR_OPERATOR, "^", 1, 29));
    CHECK_EQ(tokens[12], Token(TokenType::BITWISE_NOT_OPERATOR, "~", 1, 31));
    CHECK_EQ(tokens[13], Token(TokenType::LESS_THAN_OPERATOR, "<", 1, 33));
    CHECK_EQ(tokens[14], Token(TokenType::LESS_THAN_EQUAL_OPERATOR, "<=", 1, 36));
    CHECK_EQ(tokens[15], Token(TokenType::GREATER_THAN_OPERATOR, ">", 1, 38));
    CHECK_EQ(tokens[16], Token(TokenType::GREATER_THAN_EQUAL_OPERATOR, ">=", 1, 41));
    CHECK_EQ(tokens[17], Token(TokenType::EQUAL_OPERATOR, "=", 1, 43));
    CHECK_EQ(tokens[18], Token(TokenType::NOT_EQUAL_OPERATOR, "!=", 1, 46));
    CHECK_EQ(tokens[19], Token(TokenType::END_OF_FILE, "", 1, 47));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer keywords") {
    ErrorManager error_manager;
    Lexer lexer(
        "for repeat while if else next stop return in .. function int float string bool void array",
        &error_manager);

    // Check all the keywords
    std::vector<Token> tokens = lexer.parse_tokens();
    REQUIRE_EQ(tokens.size(), 18);
    CHECK_EQ(tokens[0], Token(TokenType::FOR_KEYWORD, "for", 1, 3));
    CHECK_EQ(tokens[1], Token(TokenType::REPEAT_KEYWORD, "repeat", 1, 10));
    CHECK_EQ(tokens[2], Token(TokenType::WHILE_KEYWORD, "while", 1, 16));
    CHECK_EQ(tokens[3], Token(TokenType::IF_KEYWORD, "if", 1, 19));
    CHECK_EQ(tokens[4], Token(TokenType::ELSE_KEYWORD, "else", 1, 24));
    CHECK_EQ(tokens[5], Token(TokenType::CONTINUE_KEYWORD, "next", 1, 29));
    CHECK_EQ(tokens[6], Token(TokenType::BREAK_KEYWORD, "stop", 1, 34));
    CHECK_EQ(tokens[7], Token(TokenType::RETURN_KEYWORD, "return", 1, 41));
    CHECK_EQ(tokens[8], Token(TokenType::IN_KEYWORD, "in", 1, 44));
    CHECK_EQ(tokens[9], Token(TokenType::RANGE_SYMBOL, "..", 1, 47));
    CHECK_EQ(tokens[10], Token(TokenType::FUNCTION_KEYWORD, "function", 1, 56));
    CHECK_EQ(tokens[11], Token(TokenType::INT_TYPE, "int", 1, 60));
    CHECK_EQ(tokens[12], Token(TokenType::FLOAT_TYPE, "float", 1, 66));
    CHECK_EQ(tokens[13], Token(TokenType::STRING_TYPE, "string", 1, 73));
    CHECK_EQ(tokens[14], Token(TokenType::BOOL_TYPE, "bool", 1, 78));
    CHECK_EQ(tokens[15], Token(TokenType::VOID_TYPE, "void", 1, 83));
    CHECK_EQ(tokens[16], Token(TokenType::ARRAY_TYPE, "array", 1, 89));
    CHECK_EQ(tokens[17], Token(TokenType::END_OF_FILE, "", 1, 90));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer newlines") {
    ErrorManager error_manager;
    Lexer lexer("1\n2\n3\\\n", &error_manager);

    // Handles newlines and escaped new lines
    std::vector<Token> tokens = lexer.parse_tokens();
    REQUIRE_EQ(tokens.size(), 6);
    CHECK_EQ(tokens[0], Token(TokenType::INT_LITERAL, "1", 1, 1));
    CHECK_EQ(tokens[1], Token(TokenType::NEW_LINE, "\n", 1, 2));
    CHECK_EQ(tokens[2], Token(TokenType::INT_LITERAL, "2", 2, 1));
    CHECK_EQ(tokens[3], Token(TokenType::NEW_LINE, "\n", 2, 2));
    CHECK_EQ(tokens[4], Token(TokenType::INT_LITERAL, "3", 3, 1));
    CHECK_EQ(tokens[5], Token(TokenType::END_OF_FILE, "", 3, 4));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer syntax") {
    ErrorManager error_manager;
    Lexer lexer("(){}[],({[, ,,]}){\n}", &error_manager);

    // Handles brackets and commas
    std::vector<Token> tokens = lexer.parse_tokens();
    REQUIRE_EQ(tokens.size(), 20);
    CHECK_EQ(tokens[0], Token(TokenType::LPAREN, "(", 1, 1));
    CHECK_EQ(tokens[1], Token(TokenType::RPAREN, ")", 1, 2));
    CHECK_EQ(tokens[2], Token(TokenType::LBRACE, "{", 1, 3));
    CHECK_EQ(tokens[3], Token(TokenType::RBRACE, "}", 1, 4));
    CHECK_EQ(tokens[4], Token(TokenType::LBRACKET, "[", 1, 5));
    CHECK_EQ(tokens[5], Token(TokenType::RBRACKET, "]", 1, 6));
    CHECK_EQ(tokens[6], Token(TokenType::COMMA, ",", 1, 7));
    CHECK_EQ(tokens[7], Token(TokenType::LPAREN, "(", 1, 8));
    CHECK_EQ(tokens[8], Token(TokenType::LBRACE, "{", 1, 9));
    CHECK_EQ(tokens[9], Token(TokenType::LBRACKET, "[", 1, 10));
    CHECK_EQ(tokens[10], Token(TokenType::COMMA, ",", 1, 11));
    CHECK_EQ(tokens[11], Token(TokenType::COMMA, ",", 1, 13));
    CHECK_EQ(tokens[12], Token(TokenType::COMMA, ",", 1, 14));
    CHECK_EQ(tokens[13], Token(TokenType::RBRACKET, "]", 1, 15));
    CHECK_EQ(tokens[14], Token(TokenType::RBRACE, "}", 1, 16));
    CHECK_EQ(tokens[15], Token(TokenType::RPAREN, ")", 1, 17));
    CHECK_EQ(tokens[16], Token(TokenType::LBRACE, "{", 1, 18));
    CHECK_EQ(tokens[17], Token(TokenType::NEW_LINE, "\n", 1, 19));
    CHECK_EQ(tokens[18], Token(TokenType::RBRACE, "}", 2, 1));
    CHECK_EQ(tokens[19], Token(TokenType::END_OF_FILE, "", 2, 2));
    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Lexer identifiers") {
    ErrorManager error_manager;
    Lexer lexer("name name_2 NAME3 4name ifname name\nmore if if2", &error_manager);

    std::vector<Token> tokens = lexer.parse_tokens();
    for (const auto &token : tokens) {
        std::cout << token.value << std::endl;
    }

    // Handles identifiers correctly with literals and keywords
    REQUIRE_EQ(tokens.size(), 12);
    CHECK_EQ(tokens[0], Token(TokenType::IDENTIFIER, "name", 1, 4));
    CHECK_EQ(tokens[1], Token(TokenType::IDENTIFIER, "name_2", 1, 11));
    CHECK_EQ(tokens[2], Token(TokenType::IDENTIFIER, "NAME3", 1, 17));
    CHECK_EQ(tokens[3], Token(TokenType::INT_LITERAL, "4", 1, 19));
    CHECK_EQ(tokens[4], Token(TokenType::IDENTIFIER, "name", 1, 23));
    CHECK_EQ(tokens[5], Token(TokenType::IDENTIFIER, "ifname", 1, 30));
    CHECK_EQ(tokens[6], Token(TokenType::IDENTIFIER, "name", 1, 35));
    CHECK_EQ(tokens[7], Token(TokenType::NEW_LINE, "\n", 1, 36));
    CHECK_EQ(tokens[8], Token(TokenType::IDENTIFIER, "more", 2, 4));
    CHECK_EQ(tokens[9], Token(TokenType::IF_KEYWORD, "if", 2, 7));
    CHECK_EQ(tokens[10], Token(TokenType::IDENTIFIER, "if2", 2, 11));
    CHECK_EQ(tokens[11], Token(TokenType::END_OF_FILE, "", 2, 12));
    CHECK_FALSE(error_manager.check_error());
}
