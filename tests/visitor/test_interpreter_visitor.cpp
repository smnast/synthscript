#include "error_manager.h"
#include "utils/shortcuts.h"
#include "utils/stream_redirect.h"
#include "utils/temp_file.h"
#include "visitor/interpreter_visitor.h"
#include <doctest/doctest.h>
#include <fstream>
#include <sstream>

TEST_CASE("Interpreter empty program") {
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "");
    InterpreterVisitor visitor(root, &error_manager);

    // Analyzes empty program without error
    visitor.interpret();
    CHECK_FALSE(error_manager.check_error());

    delete root;
}

TEST_CASE("Interpreter simple output") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "output(42)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets simple output command without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "42\n");

    delete root;
}

TEST_CASE("Interpreter simple output with expression") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "output(2 + 2)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets simple output command with expression without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "4\n");

    delete root;
}

TEST_CASE("Interpreter output with complex expressions") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "output(1 * (2 + 3 * 4) / 5 - 6 % 4)\n"
                                      "output((((1 + 2) * 3) + 4) / 5)\n"
                                      "output(1 + 2 * 3 - 4 / 5 + 6 % 7)"
                                      "output(1 * 2 * 3 * 4 * 5 * 6 * 7)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets output command with operations without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "0\n2\n13\n5040\n");

    delete root;
}

TEST_CASE("Interpreter output literals") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "output(127)\n"
                                      "output(3.14159)\n"
                                      "output(\"some string\")\n"
                                      "output(true)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets output command with literals without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "127\n3.14159\nsome string\ntrue\n");

    delete root;
}

TEST_CASE("Interpreter simple input") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- input()\n"
                                      "output(a)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets input command without error
    stream_redirect.give_string("42\n");
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "42\n");

    delete root;
}

TEST_CASE("Interpreter casting string to int") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- int(input())\n"
                                      "output(a + 1)");
    InterpreterVisitor visitor(root, &error_manager);

    // Casts input string to integer without error
    stream_redirect.give_string("41\n");
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "42\n");

    delete root;
}

TEST_CASE("Interpreter casting string to float") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- float(input())\n"
                                      "output(a + 1)");
    InterpreterVisitor visitor(root, &error_manager);

    // Casts input string to float without error
    stream_redirect.give_string("2.14159\n");
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "3.14159\n");

    delete root;
}

TEST_CASE("Interpreter boolean conditions") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- 3\n"
                                      "b <- 5\n"
                                      "c <- \"pancake\"\n"
                                      "output(a = 3)\n"
                                      "output(b != 5)\n"
                                      "output(c = \"pancake\")\n"
                                      "output(a < b)\n"
                                      "output(a > b)\n"
                                      "output(a <= b)\n"
                                      "output(a >= b)\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets boolean condition without error
    stream_redirect.give_string("42\n");
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "true\nfalse\ntrue\ntrue\nfalse\ntrue\nfalse\n");

    delete root;
}

TEST_CASE("Interpreter if statement") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager,
                      "test.txt",
                      "a <- 3\n"
                      "if a = 3 {output(42)}\n"
                      "if a = 4 {output(42)}\n"
                      "if a = 3 {output(42)} else {output(17)}\n"
                      "if a = 4 {output(42)} else if a = 3 {output(17)}\n"
                      "if a = 4 {output(42)} else if a = 20 {output(17)} else {output(102)}\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets if statement without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "42\n42\n17\n102\n");

    delete root;
}

TEST_CASE("Interpreter while statement") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- 0\n"
                                      "while a < 5 {output(a)\na <- a + 1}");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets while statement without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "0\n1\n2\n3\n4\n");

    delete root;
}

TEST_CASE("Interpreter arrays") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- [[1, 2, [3, 4, 5], 6, [7]], 8, [9, 10]]"
                                      "output(a)\n"
                                      "output(a[0])\n"
                                      "output(a[1])\n"
                                      "output(a[2][0])\n"
                                      "output(a[0][1])\n"
                                      "output(a[0][2][0])\n"
                                      "a[0] <- [1, 2, [3]]\n"
                                      "output(a)\n"
                                      "output(a[0])\n"
                                      "output(a[1])\n"
                                      "output(a[2][0])\n"
                                      "output(a[0][1])\n"
                                      "output(a[0][2][0])\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Array shenanigans
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(),
             "[[1, 2, [3, 4, 5], 6, [7]], 8, [9, 10]]\n"
             "[1, 2, [3, 4, 5], 6, [7]]\n"
             "8\n"
             "9\n"
             "2\n"
             "3\n"
             "[[1, 2, [3]], 8, [9, 10]]\n"
             "[1, 2, [3]]\n"
             "8\n"
             "9\n"
             "2\n"
             "3\n");

    delete root;
}

TEST_CASE("Interpreter strings") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- \"hello\"\n"
                                      "output(a)\n"
                                      "output(a + \" world\")\n"
                                      "output(\"hello\" + \" world\")\n"
                                      "output(\"hello\" + string(42))\n"
                                      "output(\"42\" + \"hello\")\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets string operations without error
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(),
             "hello\n"
             "hello world\n"
             "hello world\n"
             "hello42\n"
             "42hello\n");

    delete root;
}

TEST_CASE("Interpreter range literal") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "output(1..10)\n"
                                      "output(10..1)\n"
                                      "output(-10..10)\n"
                                      "output(10..-10)\n"
                                      "output(1..1)\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets range literals
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(),
             "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]\n"
             "[10, 9, 8, 7, 6, 5, 4, 3, 2, 1]\n"
             "[-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]\n"
             "[10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10]\n"
             "[1]\n");

    delete root;
}

TEST_CASE("Interpreter repeat loop") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "repeat 5 {output(42)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets repeat loop
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "42\n42\n42\n42\n42\n");

    delete root;
}

TEST_CASE("Interpreter for loop") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "for i in 1..10 {output(i)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets for loop
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");

    delete root;
}

TEST_CASE("Interpreter nested for loop") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "for i in 1..3 {for j in 1..3 {output(i + j)}}");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets nested for loop
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "2\n3\n4\n3\n4\n5\n4\n5\n6\n");

    delete root;
}

TEST_CASE("Interpreter string and array multiplication") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "output(\"hello\" * 3)\n"
                                      "output([1, 2, 3] * 3)\n"
                                      "output([1, 2, 3] * 2)\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets string and array multiplication
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(),
             "hellohellohello\n"
             "[1, 2, 3, 1, 2, 3, 1, 2, 3]\n"
             "[1, 2, 3, 1, 2, 3]\n");

    delete root;
}

TEST_CASE("Interpreter print triangle") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "for i in 1..5 {output(\"*\" * i)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets print triangle program
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "*\n**\n***\n****\n*****\n");

    delete root;
}

TEST_CASE("Interpreter simple function") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "add <- function(a, b) {return a + b}\n"
                                      "output(add(3, 4))");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets simple function
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "7\n");

    delete root;
}

TEST_CASE("Interpreter swap functions") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "add <- function(a, b) {return a + b}\n"
                                      "sub <- function(a, b) {return a - b}\n"
                                      "output(add(3, 4))\n"
                                      "output(sub(3, 4))\n"
                                      "temp <- add\n"
                                      "add <- sub\n"
                                      "sub <- temp\n"
                                      "output(add(3, 4))\n"
                                      "output(sub(3, 4))\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets swap function
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "7\n-1\n-1\n7\n");

    delete root;
}

TEST_CASE("Interpreter nested function calls") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "add <- function(a, b) {return a + b}\n"
                                      "sub <- function(a, b) {return a - b}\n"
                                      "mul <- function(a, b) {return a * b}\n"
                                      "div <- function(a, b) {return a / b}\n"
                                      "output(add(3, mul(2, sub(5, div(10, 2)))) + 1)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets nested functions
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "4\n");

    delete root;
}

TEST_CASE("Interpreter recursive function") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(
        &error_manager,
        "test.txt",
        "fib <- function(n) {if n <= 1 {return n} else {return fib(n - 1) + fib(n - 2)}}\n"
        "output(fib(10))");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets recursive function
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "55\n");

    delete root;
}

TEST_CASE("Interpreter function with no arguments") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "hello <- function() {output(\"hello\")}\n"
                                      "hello()");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets function with no arguments
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "hello\n");

    delete root;
}

TEST_CASE("Interpreter function return string") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "hello <- function() {return \"hello\"}\n"
                                      "output(hello())");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets function with return statement
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "hello\n");

    delete root;
}

TEST_CASE("Interpreter function return void") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "hello <- function() {}\nhello()\na <- hello()");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets function with return statement
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Invalid assignment to void (line 3, column 1)\n");

    delete root;
}

TEST_CASE("Interpreter compound statement scopes") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- 1\n"
                                      "b <- 1\n"
                                      "c <- 1\n"
                                      "{a <- 10 {b <- 10\nd <- 20\n{c <- "
                                      "10\noutput(a+b+c+d)}\noutput(a+b+c+d)}\noutput(a+b+c)\n}\n"
                                      "output(a)\n"
                                      "output(b)\n"
                                      "output(c)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets scopes
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "50\n50\n30\n10\n10\n10\n");

    delete root;
}

TEST_CASE("Interpreter file read") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    TempFile temp_file("text.txt", "line 1\nline 2\nline 3");
    ProgramNode *root = parse_program(
        &error_manager, "test.txt", "file_text <- read(\"text.txt\")\noutput(file_text)");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets file io
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "line 1\nline 2\nline 3\n");

    delete root;
}

TEST_CASE("Interpreter file write") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    TempFile temp_file("text.txt", "");
    ProgramNode *root = parse_program(
        &error_manager, "test.txt", "write(\"text.txt\", \"line 1\nline 2\nline 3\")");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets file io
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());

    std::ifstream stream("text.txt");
    std::stringstream buffer;
    buffer << stream.rdbuf();
    CHECK_EQ(buffer.str(), "line 1\nline 2\nline 3");

    delete root;
}

TEST_CASE("Interpreter file append") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    TempFile temp_file("text.txt", "This is the ");
    ProgramNode *root =
        parse_program(&error_manager, "test.txt", "append(\"text.txt\", \"first line.\")");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets file io
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());

    std::ifstream stream("text.txt");
    std::stringstream buffer;
    buffer << stream.rdbuf();
    CHECK_EQ(buffer.str(), "This is the first line.");

    delete root;
}

TEST_CASE("Interpreter list functions") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- [1, 2, 3, 4, 5]\n"
                                      "output(len(a))\n"
                                      "output(sum(a))\n"
                                      "output(product(a))\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets list functions
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "5\n15\n120\n");
}

TEST_CASE("Interpreter string functions") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager,
                                      "test.txt",
                                      "a <- \"abcdef g\"\n"
                                      "output(len(a))\n");
    InterpreterVisitor visitor(root, &error_manager);

    // Interprets list functions
    stream_redirect.run([&]() { visitor.interpret(); });
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(stream_redirect.get_string(), "8\n");
}

TEST_CASE("Interpreter if statement type error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "if 1 {output(42)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Runtime error when if statement condition is not of type bool
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Invalid type for if condition (expected bool, got int) (line 1, "
             "column 4)\n");
}

TEST_CASE("Interpreter for statement type error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "for i in 1 {output(i)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Runtime error when for statement range is not of type range
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Invalid type for iterable (expected array or string, got int) (line "
             "1, column 10)\n");
}

TEST_CASE("Interpreter while statement type error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "while 1 {output(42)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Runtime error when while statement condition is not of type bool
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Invalid type for while condition (expected bool, got int) (line 1, "
             "column 7)\n");
}

TEST_CASE("Interpreter repeat statement type error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "repeat true {output(42)}");
    InterpreterVisitor visitor(root, &error_manager);

    // Runtime error when repeat statement count is not of type int
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Invalid type for repeat count (expected int, got bool) (line 1, "
             "column 11)\n");
}

TEST_CASE("Interpreter built-in function argument count error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "output(42, 42)");
    InterpreterVisitor visitor(root, &error_manager);

    // Runtime error when function call has too many arguments
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Incorrect number of arguments to function 'output' (expected 1, given "
             "2) (line 1, column 6)\n");
}

TEST_CASE("Interpreter function argument count error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    ProgramNode *root = parse_program(&error_manager, "test.txt", "a <- function(a) {}\na(42, 42)");
    InterpreterVisitor visitor(root, &error_manager);

    // Runtime error when function call has too many arguments
    stream_redirect.run([&]() {
        try {
            visitor.interpret();
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Incorrect number of arguments to function 'a' (expected 1, given 2) "
             "(line 2, column 1)\n");
}
