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

TEST_CASE("Parser assignment") {
    ErrorManager error_manager;
    std::vector<Token> tokens = lex_tokens(&error_manager, "test_parser", "a <- b <- 3 + 4 * 5");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);

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
    std::vector<Token> tokens = lex_tokens(&error_manager, "test_parser", "a <- 3\nb <- a\n\nc <- \\\n3");
    Parser parser(tokens, &error_manager);

    ProgramNode *program = parser.parse_program();
    REQUIRE_NE(program, nullptr);

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

// TEST_CASE("Parser literals") {
//     // test all the literals
// }

// ... more to come
