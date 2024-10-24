#include "error_manager.h"
#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "utils/cout_redirect.h"
#include "utils/temp_file.h"
#include "visitor/semantic_analysis_visitor.h"
#include <doctest/doctest.h>

/**
 * @brief Parse the program and return the root node of the AST.
 * @param error_manager The error manager to use for error handling.
 * @param file_path The path to the file to parse.
 * @param code The code to parse.
 * @return A pointer to the root node of the AST.
 *
 * @note
 * The caller is responsible for deleting the returned node.
 */
ProgramNode *parse_program(ErrorManager *error_manager, std::string file_path, std::string code) {
    TempFile temp_file(file_path, code);
    Reader reader(file_path, error_manager);
    Lexer lexer(reader.read_file(), error_manager);
    Parser parser(lexer.parse_tokens(), error_manager);
    return parser.parse_program();
}

TEST_CASE("Semantic Analysis empty program") {
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes empty program without error
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis assignment") {
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- 17 + 2");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes assignment without error
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier") {
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- 13\nb <- 17 + a");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier without error
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "b <- 17 + a");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(cout_redirect.get_string(), "Error: Undeclared identifier 'a' (line 1, column 11)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in control statements") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- 5\nif a < 10 {} else if a < 20 {}\nwhile a < 10 {}\n"
                                      "for i in a {}\n repeat a {}\n");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in conditions
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in control statements") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(
        &error_manager,
        "test.txt",
        "if a < 10 {} else if a < 20 {}\nwhile a < 10 {}\nfor i in a {}\nrepeat a {}\n");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in conditions and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 5);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'a' (line 1, column 4)\n"
             "Error: Undeclared identifier 'a' (line 1, column 22)\n"
             "Error: Undeclared identifier 'a' (line 2, column 7)\n"
             "Error: Undeclared identifier 'a' (line 3, column 10)\n"
             "Error: Undeclared identifier 'a' (line 4, column 8)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in function call") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(
        &error_manager,
        "test.txt",
        "do_stuff <- function(a, b, c) {}\na <- 2\nb <- 3\nc <- 4\ndo_stuff(a, b, c)");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in function call
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in function call") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "do_stuff(a, b, c)");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in function call and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 4);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'do_stuff' (line 1, column 8)\n"
             "Error: Undeclared identifier 'a' (line 1, column 10)\n"
             "Error: Undeclared identifier 'b' (line 1, column 13)\n"
             "Error: Undeclared identifier 'c' (line 1, column 16)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in array subscript") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- [1, 2, 3]\nb <- 2\na[b]");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in array subscript
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in array subscript") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a[b]");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in array subscript and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 2);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'a' (line 1, column 1)\n"
             "Error: Undeclared identifier 'b' (line 1, column 3)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in operations") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- 2\nb <- 5\n -a + b");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in operations
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in operations") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "-a + b");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in operations and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 2);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'a' (line 1, column 2)\n"
             "Error: Undeclared identifier 'b' (line 1, column 6)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in range literal") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- 1\nb <- 10\na..b");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in range literal
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in range literal") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a..b");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in range literal and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 2);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'a' (line 1, column 1)\n"
             "Error: Undeclared identifier 'b' (line 1, column 4)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in array literal") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- 1\nb <- 2\n[a, b]");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in array literal
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in array literal") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "[a, b]");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in array literal and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 2);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'a' (line 1, column 2)\n"
             "Error: Undeclared identifier 'b' (line 1, column 5)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in cast") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- 3.14159\nint(a)");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in cast
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in cast") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "int(a)");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in cast and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(cout_redirect.get_string(), "Error: Undeclared identifier 'a' (line 1, column 5)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in control bodies") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(
        &error_manager,
        "test.txt",
        "a <- 2\nif true {a}\n while true {a} \n for i in 1..10 {a}\n repeat 10 {a}\n");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in control bodies
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in control bodies") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager,
                      "test.txt",
                      "if true {a}\nwhile true {a}\nfor i in 1..10 {a}\nrepeat 10 {a}\n");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in control bodies and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 4);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: Undeclared identifier 'a' (line 1, column 10)\n"
             "Error: Undeclared identifier 'a' (line 2, column 13)\n"
             "Error: Undeclared identifier 'a' (line 3, column 17)\n"
             "Error: Undeclared identifier 'a' (line 4, column 12)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in function declaration") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "do_stuff <- function(a, b) {a + b}");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in function declaration
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in function declaration") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "do_stuff <- function(a, b) {c}");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in function declaration and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(cout_redirect.get_string(), "Error: Undeclared identifier 'c' (line 1, column 29)\n");

    delete root;
}

TEST_CASE("Semantic Analysis declared identifier in return statement") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "do_stuff <- function(a, b) {return a + b}");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifier in return statement
    visitor.analyze();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Semantic Analysis undeclared identifier in return statement") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "do_stuff <- function(a, b) {return c}");
    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with undeclared identifier in return statement and reports error
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(cout_redirect.get_string(), "Error: Undeclared identifier 'c' (line 1, column 36)\n");

    delete root;
}

TEST_CASE("Semantic Analysis control statement contexts") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "next\n"
                                      "stop\n"
                                      "return\n"
                                      "if true {next}\n"
                                      "if true {stop}\n"
                                      "if true {return}\n"
                                      "for i in 1..10 {next}\n"
                                      "for i in 1..10 {stop}\n"
                                      "for i in 1..10 {return}\n"
                                      "while true {next}\n"
                                      "while true {stop}\n"
                                      "while true {return}\n"
                                      "repeat 10 {next}\n"
                                      "repeat 10 {stop}\n"
                                      "repeat 10 {return}\n"
                                      "function() {next}\n"
                                      "function() {stop}\n"
                                      "function() {return}\n");

    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with control statements in different contexts
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 11);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: 'next' statement outside of loop (line 1, column 4)\n"
             "Error: 'stop' statement outside of loop (line 2, column 4)\n"
             "Error: 'return' statement outside of function (line 3, column 6)\n"
             "Error: 'next' statement outside of loop (line 4, column 13)\n"
             "Error: 'stop' statement outside of loop (line 5, column 13)\n"
             "Error: 'return' statement outside of function (line 6, column 15)\n"
             "Error: 'return' statement outside of function (line 9, column 22)\n"
             "Error: 'return' statement outside of function (line 12, column 18)\n"
             "Error: 'return' statement outside of function (line 15, column 17)\n"
             "Error: 'next' statement outside of loop (line 16, column 16)\n"
             "Error: 'stop' statement outside of loop (line 17, column 16)\n");

    delete root;
}

TEST_CASE("Semantic Analysis control statement contexts with nested control statements") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "if true {if true {next}}\n"
                                      "if true {if true {stop}}\n"
                                      "if true {if true {return}}\n"
                                      "for i in 1..10 {for j in 1..10 {next}}\n"
                                      "for i in 1..10 {for j in 1..10 {stop}}\n"
                                      "for i in 1..10 {for j in 1..10 {return}}\n"
                                      "while true {while true {next}}\n"
                                      "while true {while true {stop}}\n"
                                      "while true {while true {return}}\n"
                                      "repeat 10 {repeat 10 {next}}\n"
                                      "repeat 10 {repeat 10 {stop}}\n"
                                      "repeat 10 {repeat 10 {return}}\n"
                                      "function() {function() {next}}\n"
                                      "function() {function() {stop}}\n"
                                      "function() {function() {return}}\n");

    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with nested control statements in different contexts
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 8);
    CHECK_EQ(cout_redirect.get_string(),
             "Error: 'next' statement outside of loop (line 1, column 22)\n"
             "Error: 'stop' statement outside of loop (line 2, column 22)\n"
             "Error: 'return' statement outside of function (line 3, column 24)\n"
             "Error: 'return' statement outside of function (line 6, column 38)\n"
             "Error: 'return' statement outside of function (line 9, column 30)\n"
             "Error: 'return' statement outside of function (line 12, column 28)\n"
             "Error: 'next' statement outside of loop (line 13, column 28)\n"
             "Error: 'stop' statement outside of loop (line 14, column 28)\n");

    delete root;
}

TEST_CASE("Semantic Analysis scopes") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- 1\n"
                                      "{a}\n"
                                      "{b <- 2}\n"
                                      "if true {a}\n"
                                      "if true {b <- 2}\n"
                                      "if b <- 2 {b}\n"
                                      "while true {a}\n"
                                      "while true {b <- 2}\n"
                                      "while b <- 2 {b}\n"
                                      "for i in 1..10 {a}\n"
                                      "for i in 1..10 {b <- 2}\n"
                                      "for i in 1..10 {i}\n"
                                      "repeat 10 {a}\n"
                                      "repeat 10 {b <- 2}\n"
                                      "b");

    SemanticAnalysisVisitor visitor(root, &error_manager);

    // Analyzes code with declared identifiers in different scopes
    cout_redirect.run([&]() { visitor.analyze(); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(cout_redirect.get_string(), "Error: Undeclared identifier 'b' (line 15, column 1)\n");

    delete root;
}
