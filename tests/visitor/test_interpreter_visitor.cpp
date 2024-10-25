#include "error_manager.h"
#include "utils/shortcuts.h"
#include "utils/stream_redirect.h"
#include "visitor/interpreter_visitor.h"
#include <doctest/doctest.h>

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
}
