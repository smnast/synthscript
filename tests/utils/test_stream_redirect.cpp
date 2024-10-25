#include "stream_redirect.h"
#include <doctest/doctest.h>
#include <iostream>

TEST_CASE("StreamRedirect initialization") {
    StreamRedirect stream_redirect;

    // Begins with empty string.
    CHECK(stream_redirect.get_string() == "");
}

TEST_CASE("StreamRedirect begin and end") {
    StreamRedirect stream_redirect;

    // Begin and end redirect cout.
    stream_redirect.begin();
    std::cout << "Line 1" << std::endl;
    std::cout << "Line 2" << std::endl;
    std::cout << "Line 3" << std::endl;
    stream_redirect.end();
    CHECK(stream_redirect.get_string() == "Line 1\nLine 2\nLine 3\n");
}

TEST_CASE("StreamRedirect run") {
    StreamRedirect stream_redirect;

    // Run a function with cout redirected.
    stream_redirect.run([]() { std::cout << "Redirected." << std::endl; });
    CHECK(stream_redirect.get_string() == "Redirected.\n");
}

TEST_CASE("StreamRedirect multiple calls") {
    StreamRedirect stream_redirect;

    // Run multiple times.
    stream_redirect.run([]() { std::cout << "First." << std::endl; });
    CHECK(stream_redirect.get_string() == "First.\n");
    stream_redirect.run([]() { std::cout << "Second." << std::endl; });
    CHECK(stream_redirect.get_string() == "First.\nSecond.\n");

    // Run multiple times with clear.
    stream_redirect.clear();
    stream_redirect.run([]() { std::cout << "Third." << std::endl; });
    CHECK(stream_redirect.get_string() == "Third.\n");
    stream_redirect.clear();
    stream_redirect.run([]() { std::cout << "Fourth." << std::endl; });
    CHECK(stream_redirect.get_string() == "Fourth.\n");
}

TEST_CASE("StreamRedirect input with begin and end") {
    StreamRedirect stream_redirect;

    // Redirect cin.
    stream_redirect.give_string("Hello, std::cin!");
    stream_redirect.begin();
    std::string word1, word2;
    std::cin >> word1 >> word2;
    stream_redirect.end();
    CHECK(word1 == "Hello,");
    CHECK(word2 == "std::cin!");
}

TEST_CASE("StreamRedirect input with run") {
    StreamRedirect stream_redirect;

    // Redirect cin with run.
    stream_redirect.give_string("Hello, std::cin!");
    stream_redirect.run([]() {
        std::string word1, word2;
        std::cin >> word1 >> word2;
        CHECK(word1 == "Hello,");
        CHECK(word2 == "std::cin!");
    });
}

TEST_CASE("StreamRedirect output the input") {
    StreamRedirect stream_redirect;

    // Input to string and output the string
    stream_redirect.give_string("This is a longer line with many words.\n");
    stream_redirect.run([]() {
        std::string line;
        std::getline(std::cin, line);
        std::cout << line << std::endl;
    }); 

    CHECK(stream_redirect.get_string() == "This is a longer line with many words.\n");
}

TEST_CASE("StreamRedirect output the input many times") {
    StreamRedirect stream_redirect;

    // 10x Input to string and output the string
    stream_redirect.give_string("This is a longer line with many words.\n");
    stream_redirect.run([&]() {
        for (int i = 0; i < 10; i++) {
            std::string line;
            std::getline(std::cin, line);
            stream_redirect.give_string(line + "\n");
        }

        std::string line;
        std::getline(std::cin, line);
        std::cout << line << std::endl;
    }); 

    CHECK(stream_redirect.get_string() == "This is a longer line with many words.\n");
}
