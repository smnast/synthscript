#include "error_manager.h"
#include "utils/cout_redirect.h"
#include <doctest/doctest.h>

TEST_CASE("ErrorManager initialization") {
    ErrorManager error_manager;
    CHECK_FALSE(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 0);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::SUCCESS);
}

TEST_CASE("ErrorManager add error") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    // Add an error message.
    cout_redirect.run([&]() { error_manager.error("Error message", false); });
    CHECK(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 1);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message\n");

    // Add another error message, without handling the first.
    cout_redirect.run([&]() { error_manager.error("Error message 2", false); });
    CHECK(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 1);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message\n");
}

TEST_CASE("ErrorManager handle error") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    // Add an error message.
    cout_redirect.run([&]() { error_manager.error("Error message", false); });

    // Handle the error.
    error_manager.handle_error();
    CHECK_FALSE(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 1);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message\n");

    // Add another error message.
    cout_redirect.run([&]() { error_manager.error("Error message 2", false); });
    CHECK(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 2);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message\nError: Error message 2\n");
}

TEST_CASE("ErrorManager force print") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    // Add an error message.
    cout_redirect.run([&]() { error_manager.error("Error message", false); });
    
    // Add another error message, forcing it to print.
    cout_redirect.run([&]() { error_manager.error("Error message 2", true); });
    CHECK(cout_redirect.get_string() == "Error: Error message\nError: Error message 2\n");
}

TEST_CASE("ErrorManager error at pos") {
    CoutRedirect cout_redirect;
    ErrorManager error_manager;

    // Set file lines
    std::vector<std::string> lines = {
        "line 1\n",
        "line 2\n",
        "line 3\n"
    };
    error_manager.set_file_lines(lines);

    // Add an error message at a position.
    cout_redirect.run([&]() { error_manager.error_at_pos("Error message", 1, 2, false); });
    CHECK(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 1);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message\nline 1\n ^\n");

    // Add another error message at a different position.
    cout_redirect.run([&]() { error_manager.error_at_pos("Error message 2", 2, 3, false); });
    CHECK(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 1);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message\nline 1\n ^\n");

    // Handle the error.
    error_manager.handle_error();
    CHECK_FALSE(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 1);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    
    // Add another error message at a different position.
    cout_redirect.clear();

    cout_redirect.run([&]() { error_manager.error_at_pos("Error message 3", 3, 4, false); });
    CHECK(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 2);
    CHECK(error_manager.get_status() == ErrorManager::BuildStatus::FAILURE);
    CHECK(cout_redirect.get_string() == "Error: Error message 3\nline 3\n   ^\n");
}