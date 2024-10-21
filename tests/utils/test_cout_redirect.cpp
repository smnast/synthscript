#include "test_output_utils.h"
#include <doctest/doctest.h>
#include <iostream>

TEST_CASE("CoutRedirect initialization") {
    CoutRedirect cout_redirect;
    
    // Begins with empty string.
    CHECK(cout_redirect.get_string() == "");
}

TEST_CASE("CoutRedirect begin and end") {
    CoutRedirect cout_redirect;
    
    // Begin and end redirect cout.
    cout_redirect.begin();
    std::cout << "Line 1" << std::endl;
    std::cout << "Line 2" << std::endl;
    std::cout << "Line 3" << std::endl;
    cout_redirect.end();
    CHECK(cout_redirect.get_string() == "Line 1\nLine 2\nLine 3\n");
}

TEST_CASE("CoutRedirect run") {
    CoutRedirect cout_redirect;
    
    // Run a function with cout redirected.
    cout_redirect.run([]() { std::cout << "Redirected." << std::endl; });
    CHECK(cout_redirect.get_string() == "Redirected.\n");
}

TEST_CASE("CoutRedirect multiple calls") {
    CoutRedirect cout_redirect;

    // Run multiple times.
    cout_redirect.run([]() { std::cout << "First." << std::endl; });
    CHECK(cout_redirect.get_string() == "First.\n");
    cout_redirect.run([]() { std::cout << "Second." << std::endl; });
    CHECK(cout_redirect.get_string() == "First.\nSecond.\n");

    // Run multiple times with clear.
    cout_redirect.clear();
    cout_redirect.run([]() { std::cout << "Third." << std::endl; });
    CHECK(cout_redirect.get_string() == "Third.\n");
    cout_redirect.clear();
    cout_redirect.run([]() { std::cout << "Fourth." << std::endl; });
    CHECK(cout_redirect.get_string() == "Fourth.\n");
}
