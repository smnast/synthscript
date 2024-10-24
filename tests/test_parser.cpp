#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "utils/temp_file.h"
#include <doctest/doctest.h>

/**
 * @brief Lex the tokens of the code.
 * @param error_manager The error manager to use for error handling.
 * @param file_path The path of the file.
 * @param code The code to be lexed.
 * @return std::vector<Token> The lexed tokens.
 */
std::vector<Token>
lex_tokens(ErrorManager *error_manager, std::string file_path, std::string code) {
    TempFile temp_file(file_path, code);
    Reader reader(file_path, error_manager);
    Lexer lexer(reader.read_file(), error_manager);
    std::vector<Token> tokens = lexer.parse_tokens();
    return tokens;
}

/**
 * @brief Try to cast an ASTNode to a specific type.
 * @tparam Type to cast to.
 * @param node The node to cast.
 * @return T* The casted node.
 */
template <typename T> T *try_cast(ASTNode *node) {
    REQUIRE_NE(node, nullptr);
    REQUIRE_EQ(node->get_node_type(), T::get_node_type_static());
    return static_cast<T *>(node);
}

// TODO: for now, we ignore the lines and columns, though those should still be tested!

TEST_CASE("Parser simple arithmetic") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(&error_manager, "test_parser", "1 + 2 * 3");
    Parser parser(tokens, &error_manager);

    // The expression
    // 1 + 2 * 3
    // Should be parsed as:
    // (1) + ((2) * (3))

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *bin_op = try_cast<BinOpNode>(program->get_statement(0));
    CHECK_EQ(bin_op->get_op(), TokenType::ADDITION_OPERATOR);

    auto *left = try_cast<LiteralNode>(bin_op->get_left_node());
    CHECK_EQ(left->get_type(), Type::TYPE_INT);
    CHECK_EQ(left->get_value(), "1");

    auto *right = try_cast<BinOpNode>(bin_op->get_right_node());
    CHECK_EQ(right->get_op(), TokenType::MULTIPLICATIVE_OPERATOR);

    auto *right_left = try_cast<LiteralNode>(right->get_left_node());
    CHECK_EQ(right_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_left->get_value(), "2");

    auto *right_right = try_cast<LiteralNode>(right->get_right_node());
    CHECK_EQ(right_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_right->get_value(), "3");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser complex arithmetic") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "(1 + 2) / 3 * (9 - 5) * -1 + 76 % +3");
    Parser parser(tokens, &error_manager);

    // The expression
    // (1 + 2) / 3 * (9 - 5) * -1 + 76 % +3
    // Should be parsed as:
    // ((((1) + (2)) / (3)) * (((9) - (5)) * (-(1)))) + ((76) % (+(3)))

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *bin_op = try_cast<BinOpNode>(program->get_statement(0));
    CHECK_EQ(bin_op->get_op(), TokenType::ADDITION_OPERATOR);

    auto *left = try_cast<BinOpNode>(bin_op->get_left_node());
    CHECK_EQ(left->get_op(), TokenType::MULTIPLICATIVE_OPERATOR);

    auto *left_left = try_cast<BinOpNode>(left->get_left_node());
    CHECK_EQ(left_left->get_op(), TokenType::MULTIPLICATIVE_OPERATOR);

    auto *left_left_left = try_cast<BinOpNode>(left_left->get_left_node());
    CHECK_EQ(left_left_left->get_op(), TokenType::DIVISION_OPERATOR);

    auto *left_left_left_left = try_cast<BinOpNode>(left_left_left->get_left_node());
    CHECK_EQ(left_left_left_left->get_op(), TokenType::ADDITION_OPERATOR);

    auto *left_left_left_left_left = try_cast<LiteralNode>(left_left_left_left->get_left_node());
    CHECK_EQ(left_left_left_left_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_left_left_left_left->get_value(), "1");

    auto *left_left_left_left_right = try_cast<LiteralNode>(left_left_left_left->get_right_node());
    CHECK_EQ(left_left_left_left_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_left_left_left_right->get_value(), "2");

    auto *left_left_left_right = try_cast<LiteralNode>(left_left_left->get_right_node());
    CHECK_EQ(left_left_left_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_left_left_right->get_value(), "3");

    auto *left_left_right = try_cast<BinOpNode>(left_left->get_right_node());
    CHECK_EQ(left_left_right->get_op(), TokenType::SUBTRACTION_OPERATOR);

    auto *left_left_right_left = try_cast<LiteralNode>(left_left_right->get_left_node());
    CHECK_EQ(left_left_right_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_left_right_left->get_value(), "9");

    auto *left_left_right_right = try_cast<LiteralNode>(left_left_right->get_right_node());
    CHECK_EQ(left_left_right_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_left_right_right->get_value(), "5");

    auto *left_right = try_cast<UnaryOpNode>(left->get_right_node());
    CHECK_EQ(left_right->get_op(), TokenType::SUBTRACTION_OPERATOR);

    auto *left_right_node = try_cast<LiteralNode>(left_right->get_operand());
    CHECK_EQ(left_right_node->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_right_node->get_value(), "1");

    auto *right = try_cast<BinOpNode>(bin_op->get_right_node());
    CHECK_EQ(right->get_op(), TokenType::MOD_OPERATOR);

    auto *right_left = try_cast<LiteralNode>(right->get_left_node());
    CHECK_EQ(right_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_left->get_value(), "76");

    auto *right_right = try_cast<UnaryOpNode>(right->get_right_node());
    CHECK_EQ(right_right->get_op(), TokenType::ADDITION_OPERATOR);

    auto *right_right_node = try_cast<LiteralNode>(right_right->get_operand());
    CHECK_EQ(right_right_node->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_right_node->get_value(), "3");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser boolean operators") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "true and false or not true");
    Parser parser(tokens, &error_manager);

    // The expression
    // true and false or not true
    // Should be parsed as:
    // ((true) and (false)) or (not (true))

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *bin_op = try_cast<BinOpNode>(program->get_statement(0));
    CHECK_EQ(bin_op->get_op(), TokenType::LOGICAL_OR_OPERATOR);

    auto *left = try_cast<BinOpNode>(bin_op->get_left_node());
    CHECK_EQ(left->get_op(), TokenType::LOGICAL_AND_OPERATOR);

    auto *left_left = try_cast<LiteralNode>(left->get_left_node());
    CHECK_EQ(left_left->get_type(), Type::TYPE_BOOL);
    CHECK_EQ(left_left->get_value(), "true");

    auto *left_right = try_cast<LiteralNode>(left->get_right_node());
    CHECK_EQ(left_right->get_type(), Type::TYPE_BOOL);
    CHECK_EQ(left_right->get_value(), "false");

    auto *right = try_cast<UnaryOpNode>(bin_op->get_right_node());
    CHECK_EQ(right->get_op(), TokenType::LOGICAL_NOT_OPERATOR);

    auto *right_node = try_cast<LiteralNode>(right->get_operand());
    CHECK_EQ(right_node->get_type(), Type::TYPE_BOOL);
    CHECK_EQ(right_node->get_value(), "true");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser bitwise operators") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(&error_manager, "test_parser", "1 & 2 | 3 ^ 4");
    Parser parser(tokens, &error_manager);

    // The expression
    // 1 & 2 | 3 ^ 4
    // Should be parsed as:
    // ((1) & (2)) | ((3) ^ (4))

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *bin_op = try_cast<BinOpNode>(program->get_statement(0));
    CHECK_EQ(bin_op->get_op(), TokenType::BITWISE_OR_OPERATOR);

    auto *left = try_cast<BinOpNode>(bin_op->get_left_node());
    CHECK_EQ(left->get_op(), TokenType::BITWISE_AND_OPERATOR);

    auto *left_left = try_cast<LiteralNode>(left->get_left_node());
    CHECK_EQ(left_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_left->get_value(), "1");

    auto *left_right = try_cast<LiteralNode>(left->get_right_node());
    CHECK_EQ(left_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(left_right->get_value(), "2");

    auto *right = try_cast<BinOpNode>(bin_op->get_right_node());
    CHECK_EQ(right->get_op(), TokenType::BITWISE_XOR_OPERATOR);

    auto *right_left = try_cast<LiteralNode>(right->get_left_node());
    CHECK_EQ(right_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_left->get_value(), "3");

    auto *right_right = try_cast<LiteralNode>(right->get_right_node());
    CHECK_EQ(right_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_right->get_value(), "4");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser assignment") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(&error_manager, "test_parser", "a <- b <- 3 + 4 * 5");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *a_assignment = try_cast<AssignmentNode>(program->get_statement(0));
    auto *a_identifier = try_cast<IdentifierNode>(a_assignment->get_identifier());
    CHECK_EQ(a_identifier->get_name(), "a");

    auto *b_assignment = try_cast<AssignmentNode>(a_assignment->get_value());
    auto *b_identifier = try_cast<IdentifierNode>(b_assignment->get_identifier());
    CHECK_EQ(b_identifier->get_name(), "b");

    auto *bin_op = try_cast<BinOpNode>(b_assignment->get_value());
    CHECK_EQ(bin_op->get_op(), TokenType::ADDITION_OPERATOR);

    auto *left = try_cast<LiteralNode>(bin_op->get_left_node());
    CHECK_EQ(left->get_type(), Type::TYPE_INT);
    CHECK_EQ(left->get_value(), "3");

    auto *right = try_cast<BinOpNode>(bin_op->get_right_node());
    CHECK_EQ(right->get_op(), TokenType::MULTIPLICATIVE_OPERATOR);

    auto *right_left = try_cast<LiteralNode>(right->get_left_node());
    CHECK_EQ(right_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_left->get_value(), "4");

    auto *right_right = try_cast<LiteralNode>(right->get_right_node());
    CHECK_EQ(right_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(right_right->get_value(), "5");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser assignments with newlines") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "a <- 3\nb <- a\n\nc <- \\\n3");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 3);

    auto *a_assignment = try_cast<AssignmentNode>(program->get_statement(0));
    auto *a_identifier = try_cast<IdentifierNode>(a_assignment->get_identifier());
    CHECK_EQ(a_identifier->get_name(), "a");
    auto *a_value = try_cast<LiteralNode>(a_assignment->get_value());
    CHECK_EQ(a_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(a_value->get_value(), "3");

    auto *b_assignment = try_cast<AssignmentNode>(program->get_statement(1));
    auto *b_identifier = try_cast<IdentifierNode>(b_assignment->get_identifier());
    CHECK_EQ(b_identifier->get_name(), "b");
    auto *b_value = try_cast<IdentifierNode>(b_assignment->get_value());
    CHECK_EQ(b_value->get_name(), "a");

    auto *c_assignment = try_cast<AssignmentNode>(program->get_statement(2));
    auto *c_identifier = try_cast<IdentifierNode>(c_assignment->get_identifier());
    CHECK_EQ(c_identifier->get_name(), "c");
    auto *c_value = try_cast<LiteralNode>(c_assignment->get_value());
    CHECK_EQ(c_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(c_value->get_value(), "3");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser literals") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(
        &error_manager,
        "test_parser",
        "a <- false\nb <- \"string\"\nc <- 7123\nd <- 3.14159\ne <- [1, 2, 3]\nf <- 1..3");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 6);

    auto *a_assignment = try_cast<AssignmentNode>(program->get_statement(0));
    auto *a_identifier = try_cast<IdentifierNode>(a_assignment->get_identifier());
    CHECK_EQ(a_identifier->get_name(), "a");
    auto *a_value = try_cast<LiteralNode>(a_assignment->get_value());
    CHECK_EQ(a_value->get_type(), Type::TYPE_BOOL);
    CHECK_EQ(a_value->get_value(), "false");

    auto *b_assignment = try_cast<AssignmentNode>(program->get_statement(1));
    auto *b_identifier = try_cast<IdentifierNode>(b_assignment->get_identifier());
    CHECK_EQ(b_identifier->get_name(), "b");
    auto *b_value = try_cast<LiteralNode>(b_assignment->get_value());
    CHECK_EQ(b_value->get_type(), Type::TYPE_STRING);
    CHECK_EQ(b_value->get_value(), "\"string\"");

    auto *c_assignment = try_cast<AssignmentNode>(program->get_statement(2));
    auto *c_identifier = try_cast<IdentifierNode>(c_assignment->get_identifier());
    CHECK_EQ(c_identifier->get_name(), "c");
    auto *c_value = try_cast<LiteralNode>(c_assignment->get_value());
    CHECK_EQ(c_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(c_value->get_value(), "7123");

    auto *d_assignment = try_cast<AssignmentNode>(program->get_statement(3));
    auto *d_identifier = try_cast<IdentifierNode>(d_assignment->get_identifier());
    CHECK_EQ(d_identifier->get_name(), "d");
    auto *d_value = try_cast<LiteralNode>(d_assignment->get_value());
    CHECK_EQ(d_value->get_type(), Type::TYPE_FLOAT);
    CHECK_EQ(d_value->get_value(), "3.14159");

    auto *e_assignment = try_cast<AssignmentNode>(program->get_statement(4));
    auto *e_identifier = try_cast<IdentifierNode>(e_assignment->get_identifier());
    CHECK_EQ(e_identifier->get_name(), "e");
    auto *e_value = try_cast<ArrayLiteralNode>(e_assignment->get_value());
    REQUIRE_EQ(e_value->get_values_size(), 3);

    auto *e_value_0 = try_cast<LiteralNode>(e_value->get_value(0));
    CHECK_EQ(e_value_0->get_type(), Type::TYPE_INT);
    CHECK_EQ(e_value_0->get_value(), "1");

    auto *e_value_1 = try_cast<LiteralNode>(e_value->get_value(1));
    CHECK_EQ(e_value_1->get_type(), Type::TYPE_INT);
    CHECK_EQ(e_value_1->get_value(), "2");

    auto *e_value_2 = try_cast<LiteralNode>(e_value->get_value(2));
    CHECK_EQ(e_value_2->get_type(), Type::TYPE_INT);
    CHECK_EQ(e_value_2->get_value(), "3");

    auto *f_assignment = try_cast<AssignmentNode>(program->get_statement(5));
    auto *f_identifier = try_cast<IdentifierNode>(f_assignment->get_identifier());
    CHECK_EQ(f_identifier->get_name(), "f");
    auto *f_value = try_cast<RangeLiteralNode>(f_assignment->get_value());
    auto *start = try_cast<LiteralNode>(f_value->get_start());
    CHECK_EQ(start->get_type(), Type::TYPE_INT);
    CHECK_EQ(start->get_value(), "1");
    auto *end = try_cast<LiteralNode>(f_value->get_end());
    CHECK_EQ(end->get_type(), Type::TYPE_INT);
    CHECK_EQ(end->get_value(), "3");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser if statement") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(
        &error_manager,
        "test_parser",
        "if 3 + 4 = 7 {\n    a <- 2\n} else if true {\n    a <- 3\n} else {\n    a <- 4\n}");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *if_statement = try_cast<IfStatementNode>(program->get_statement(0));
    auto *if_condition = try_cast<BinOpNode>(if_statement->get_condition());
    CHECK_EQ(if_condition->get_op(), TokenType::EQUAL_OPERATOR);

    auto *if_condition_left = try_cast<BinOpNode>(if_condition->get_left_node());
    CHECK_EQ(if_condition_left->get_op(), TokenType::ADDITION_OPERATOR);

    auto *if_condition_left_left = try_cast<LiteralNode>(if_condition_left->get_left_node());
    CHECK_EQ(if_condition_left_left->get_type(), Type::TYPE_INT);
    CHECK_EQ(if_condition_left_left->get_value(), "3");

    auto *if_condition_left_right = try_cast<LiteralNode>(if_condition_left->get_right_node());
    CHECK_EQ(if_condition_left_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(if_condition_left_right->get_value(), "4");

    auto *if_condition_right = try_cast<LiteralNode>(if_condition->get_right_node());
    CHECK_EQ(if_condition_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(if_condition_right->get_value(), "7");

    auto *if_body = try_cast<CompoundStatementNode>(if_statement->get_if_body());
    REQUIRE_EQ(if_body->get_statements_size(), 1);

    auto *if_body_assignment = try_cast<AssignmentNode>(if_body->get_statement(0));
    auto *if_body_assignment_identifier =
        try_cast<IdentifierNode>(if_body_assignment->get_identifier());
    CHECK_EQ(if_body_assignment_identifier->get_name(), "a");
    auto *if_body_assignment_value = try_cast<LiteralNode>(if_body_assignment->get_value());
    CHECK_EQ(if_body_assignment_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(if_body_assignment_value->get_value(), "2");

    auto *else_if_statement = try_cast<IfStatementNode>(if_statement->get_else_body());
    auto *else_if_condition = try_cast<LiteralNode>(else_if_statement->get_condition());
    CHECK_EQ(else_if_condition->get_type(), Type::TYPE_BOOL);
    CHECK_EQ(else_if_condition->get_value(), "true");

    auto *else_if_body = try_cast<CompoundStatementNode>(else_if_statement->get_if_body());
    REQUIRE_EQ(else_if_body->get_statements_size(), 1);

    auto *else_if_body_assignment = try_cast<AssignmentNode>(else_if_body->get_statement(0));
    auto *else_if_body_assignment_identifier =
        try_cast<IdentifierNode>(else_if_body_assignment->get_identifier());
    CHECK_EQ(else_if_body_assignment_identifier->get_name(), "a");
    auto *else_if_body_assignment_value =
        try_cast<LiteralNode>(else_if_body_assignment->get_value());
    CHECK_EQ(else_if_body_assignment_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(else_if_body_assignment_value->get_value(), "3");

    auto *else_body = try_cast<CompoundStatementNode>(else_if_statement->get_else_body());
    REQUIRE_EQ(else_body->get_statements_size(), 1);

    auto *else_body_assignment = try_cast<AssignmentNode>(else_body->get_statement(0));
    auto *else_body_assignment_identifier =
        try_cast<IdentifierNode>(else_body_assignment->get_identifier());
    CHECK_EQ(else_body_assignment_identifier->get_name(), "a");
    auto *else_body_assignment_value = try_cast<LiteralNode>(else_body_assignment->get_value());
    CHECK_EQ(else_body_assignment_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(else_body_assignment_value->get_value(), "4");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser while statement") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "while a < 10 {\n    a <- a + 1\n}");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *while_statement = try_cast<WhileStatementNode>(program->get_statement(0));
    auto *while_condition = try_cast<BinOpNode>(while_statement->get_condition());
    CHECK_EQ(while_condition->get_op(), TokenType::LESS_THAN_OPERATOR);

    auto *while_condition_left = try_cast<IdentifierNode>(while_condition->get_left_node());
    CHECK_EQ(while_condition_left->get_name(), "a");

    auto *while_condition_right = try_cast<LiteralNode>(while_condition->get_right_node());
    CHECK_EQ(while_condition_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(while_condition_right->get_value(), "10");

    auto *while_body = try_cast<CompoundStatementNode>(while_statement->get_body());
    REQUIRE_EQ(while_body->get_statements_size(), 1);

    auto *while_body_assignment = try_cast<AssignmentNode>(while_body->get_statement(0));
    auto *while_body_assignment_identifier =
        try_cast<IdentifierNode>(while_body_assignment->get_identifier());
    CHECK_EQ(while_body_assignment_identifier->get_name(), "a");
    auto *while_body_assignment_value = try_cast<BinOpNode>(while_body_assignment->get_value());
    CHECK_EQ(while_body_assignment_value->get_op(), TokenType::ADDITION_OPERATOR);

    auto *while_body_assignment_value_left =
        try_cast<IdentifierNode>(while_body_assignment_value->get_left_node());
    CHECK_EQ(while_body_assignment_value_left->get_name(), "a");

    auto *while_body_assignment_value_right =
        try_cast<LiteralNode>(while_body_assignment_value->get_right_node());
    CHECK_EQ(while_body_assignment_value_right->get_type(), Type::TYPE_INT);
    CHECK_EQ(while_body_assignment_value_right->get_value(), "1");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser for statement") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "for i in 1..10 {\n    a <- a + i\n}");

    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *for_statement = try_cast<ForStatementNode>(program->get_statement(0));
    CHECK_EQ(for_statement->get_identifier(), "i");

    auto *for_range = try_cast<RangeLiteralNode>(for_statement->get_iterable());
    auto *for_range_start = try_cast<LiteralNode>(for_range->get_start());
    CHECK_EQ(for_range_start->get_type(), Type::TYPE_INT);
    CHECK_EQ(for_range_start->get_value(), "1");

    auto *for_range_end = try_cast<LiteralNode>(for_range->get_end());
    CHECK_EQ(for_range_end->get_type(), Type::TYPE_INT);
    CHECK_EQ(for_range_end->get_value(), "10");

    auto *for_body = try_cast<CompoundStatementNode>(for_statement->get_body());
    REQUIRE_EQ(for_body->get_statements_size(), 1);

    auto *for_body_assignment = try_cast<AssignmentNode>(for_body->get_statement(0));
    auto *for_body_assignment_identifier =
        try_cast<IdentifierNode>(for_body_assignment->get_identifier());
    CHECK_EQ(for_body_assignment_identifier->get_name(), "a");
    auto *for_body_assignment_value = try_cast<BinOpNode>(for_body_assignment->get_value());
    CHECK_EQ(for_body_assignment_value->get_op(), TokenType::ADDITION_OPERATOR);

    auto *for_body_assignment_value_left =
        try_cast<IdentifierNode>(for_body_assignment_value->get_left_node());
    CHECK_EQ(for_body_assignment_value_left->get_name(), "a");

    auto *for_body_assignment_value_right =
        try_cast<IdentifierNode>(for_body_assignment_value->get_right_node());
    CHECK_EQ(for_body_assignment_value_right->get_name(), "i");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser repeat statement") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "repeat 10 {\n    a <- 2\n}");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *repeat_statement = try_cast<RepeatStatementNode>(program->get_statement(0));
    auto *repeat_count = try_cast<LiteralNode>(repeat_statement->get_count());
    CHECK_EQ(repeat_count->get_type(), Type::TYPE_INT);
    CHECK_EQ(repeat_count->get_value(), "10");

    auto *repeat_body = try_cast<CompoundStatementNode>(repeat_statement->get_body());
    REQUIRE_EQ(repeat_body->get_statements_size(), 1);

    auto *repeat_body_assignment = try_cast<AssignmentNode>(repeat_body->get_statement(0));
    auto *repeat_body_assignment_identifier =
        try_cast<IdentifierNode>(repeat_body_assignment->get_identifier());
    CHECK_EQ(repeat_body_assignment_identifier->get_name(), "a");
    auto *repeat_body_assignment_value = try_cast<LiteralNode>(repeat_body_assignment->get_value());
    CHECK_EQ(repeat_body_assignment_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(repeat_body_assignment_value->get_value(), "2");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser function declaration") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "add <- function(a, b) {\n    return a + b\n}");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *add_assignment = try_cast<AssignmentNode>(program->get_statement(0));
    auto *add_identifier = try_cast<IdentifierNode>(add_assignment->get_identifier());
    CHECK_EQ(add_identifier->get_name(), "add");

    auto *add_value = try_cast<FunctionDeclarationNode>(add_assignment->get_value());
    CHECK_EQ(add_value->get_parameters_size(), 2);
    CHECK_EQ(add_value->get_parameter(0), "a");
    CHECK_EQ(add_value->get_parameter(1), "b");

    auto *add_body = try_cast<CompoundStatementNode>(add_value->get_body());
    REQUIRE_EQ(add_body->get_statements_size(), 1);

    auto *add_body_return = try_cast<ReturnStatementNode>(add_body->get_statement(0));
    auto *add_body_return_value = try_cast<BinOpNode>(add_body_return->get_value());
    CHECK_EQ(add_body_return_value->get_op(), TokenType::ADDITION_OPERATOR);

    auto *add_body_return_left = try_cast<IdentifierNode>(add_body_return_value->get_left_node());
    CHECK_EQ(add_body_return_left->get_name(), "a");

    auto *add_body_return_right = try_cast<IdentifierNode>(add_body_return_value->get_right_node());
    CHECK_EQ(add_body_return_right->get_name(), "b");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parse function call") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(
        &error_manager, "test_parser", "result <- do_stuff(3, 4, \"string\", true, 3.14159)");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 1);

    auto *result_assignment = try_cast<AssignmentNode>(program->get_statement(0));
    auto *result_identifier = try_cast<IdentifierNode>(result_assignment->get_identifier());
    CHECK_EQ(result_identifier->get_name(), "result");

    auto *result_value = try_cast<CallOpNode>(result_assignment->get_value());
    CHECK_EQ(result_value->get_identifier(), "do_stuff");
    CHECK_EQ(result_value->get_arguments_size(), 5);

    auto *result_value_arg0 = try_cast<LiteralNode>(result_value->get_argument(0));
    CHECK_EQ(result_value_arg0->get_type(), Type::TYPE_INT);
    CHECK_EQ(result_value_arg0->get_value(), "3");

    auto *result_value_arg1 = try_cast<LiteralNode>(result_value->get_argument(1));
    CHECK_EQ(result_value_arg1->get_type(), Type::TYPE_INT);
    CHECK_EQ(result_value_arg1->get_value(), "4");

    auto *result_value_arg2 = try_cast<LiteralNode>(result_value->get_argument(2));
    CHECK_EQ(result_value_arg2->get_type(), Type::TYPE_STRING);
    CHECK_EQ(result_value_arg2->get_value(), "\"string\"");

    auto *result_value_arg3 = try_cast<LiteralNode>(result_value->get_argument(3));
    CHECK_EQ(result_value_arg3->get_type(), Type::TYPE_BOOL);
    CHECK_EQ(result_value_arg3->get_value(), "true");

    auto *result_value_arg4 = try_cast<LiteralNode>(result_value->get_argument(4));
    CHECK_EQ(result_value_arg4->get_type(), Type::TYPE_FLOAT);
    CHECK_EQ(result_value_arg4->get_value(), "3.14159");

    CHECK_FALSE(error_manager.check_error());
}

TEST_CASE("Parser array indexing") {
    ErrorManager error_manager;
    std::vector<Token> tokens =
        lex_tokens(&error_manager, "test_parser", "a[2] <- 3\nb[2][3] <- 4\nc[2][3][4] <- 5");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);
    REQUIRE_EQ(program->get_statements_size(), 3);

    auto *a_assignment = try_cast<AssignmentNode>(program->get_statement(0));
    auto *a_subscript = try_cast<SubscriptOpNode>(a_assignment->get_identifier());
    auto *a_subscript_identifier = try_cast<IdentifierNode>(a_subscript->get_identifier());
    CHECK_EQ(a_subscript_identifier->get_name(), "a");
    auto *a_subscript_index = try_cast<LiteralNode>(a_subscript->get_index());
    CHECK_EQ(a_subscript_index->get_type(), Type::TYPE_INT);
    CHECK_EQ(a_subscript_index->get_value(), "2");

    auto *a_value = try_cast<LiteralNode>(a_assignment->get_value());
    CHECK_EQ(a_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(a_value->get_value(), "3");

    auto *b_assignment = try_cast<AssignmentNode>(program->get_statement(1));
    auto *b_subscript = try_cast<SubscriptOpNode>(b_assignment->get_identifier());
    auto *b_subscript_identifier = try_cast<SubscriptOpNode>(b_subscript->get_identifier());
    auto *b_subscript_identifier_identifier =
        try_cast<IdentifierNode>(b_subscript_identifier->get_identifier());
    CHECK_EQ(b_subscript_identifier_identifier->get_name(), "b");
    auto *b_subscript_identifier_index = try_cast<LiteralNode>(b_subscript_identifier->get_index());
    CHECK_EQ(b_subscript_identifier_index->get_type(), Type::TYPE_INT);
    CHECK_EQ(b_subscript_identifier_index->get_value(), "2");
    auto *b_subscript_index = try_cast<LiteralNode>(b_subscript->get_index());
    CHECK_EQ(b_subscript_index->get_type(), Type::TYPE_INT);
    CHECK_EQ(b_subscript_index->get_value(), "3");

    auto *b_value = try_cast<LiteralNode>(b_assignment->get_value());
    CHECK_EQ(b_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(b_value->get_value(), "4");

    auto *c_assignment = try_cast<AssignmentNode>(program->get_statement(2));
    auto *c_subscript = try_cast<SubscriptOpNode>(c_assignment->get_identifier());
    auto *c_subscript_identifier = try_cast<SubscriptOpNode>(c_subscript->get_identifier());
    auto *c_subscript_identifier_identifier =
        try_cast<SubscriptOpNode>(c_subscript_identifier->get_identifier());
    auto *c_subscript_identifier_identifier_identifier =
        try_cast<IdentifierNode>(c_subscript_identifier_identifier->get_identifier());
    CHECK_EQ(c_subscript_identifier_identifier_identifier->get_name(), "c");
    auto *c_subscript_identifier_identifier_index =
        try_cast<LiteralNode>(c_subscript_identifier_identifier->get_index());
    CHECK_EQ(c_subscript_identifier_identifier_index->get_type(), Type::TYPE_INT);
    CHECK_EQ(c_subscript_identifier_identifier_index->get_value(), "2");
    auto *c_subscript_identifier_index = try_cast<LiteralNode>(c_subscript_identifier->get_index());
    CHECK_EQ(c_subscript_identifier_index->get_type(), Type::TYPE_INT);
    CHECK_EQ(c_subscript_identifier_index->get_value(), "3");
    auto *c_subscript_index = try_cast<LiteralNode>(c_subscript->get_index());
    CHECK_EQ(c_subscript_index->get_type(), Type::TYPE_INT);
    CHECK_EQ(c_subscript_index->get_value(), "4");

    auto *c_value = try_cast<LiteralNode>(c_assignment->get_value());
    CHECK_EQ(c_value->get_type(), Type::TYPE_INT);
    CHECK_EQ(c_value->get_value(), "5");

    CHECK_FALSE(error_manager.check_error());
}
