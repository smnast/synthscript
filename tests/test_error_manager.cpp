#include "error_manager.h"
#include "utils/stream_redirect.h"
#include <doctest/doctest.h>

TEST_CASE("ErrorManager initialization") {
    ErrorManager error_manager;
    CHECK_FALSE(error_manager.check_error());
    CHECK(error_manager.get_error_count() == 0);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::SUCCESS);
}

TEST_CASE("ErrorManager add error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    // Add an error message.
    stream_redirect.run([&]() { error_manager.error("Error message", false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message\n");

    // Add another error message, without handling the first.
    stream_redirect.run([&]() { error_manager.error("Error message 2", false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message\n");
}

TEST_CASE("ErrorManager handle error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    // Add an error message.
    stream_redirect.run([&]() { error_manager.error("Error message", false); });

    // Handle the error.
    error_manager.handle_error();
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message\n");

    // Add another error message.
    stream_redirect.run([&]() { error_manager.error("Error message 2", false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 2);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message\nError: Error message 2\n");
}

TEST_CASE("ErrorManager force print") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    // Add an error message.
    stream_redirect.run([&]() { error_manager.error("Error message", false); });

    // Add another error message, forcing it to print.
    stream_redirect.run([&]() { error_manager.error("Error message 2", true); });
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message\nError: Error message 2\n");
}

TEST_CASE("ErrorManager error at pos") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    // Set file lines
    std::vector<std::string> lines = {"line 1\n", "line 2\n", "line 3\n"};
    error_manager.set_file_lines(lines);

    // Add an error message at a position.
    stream_redirect.run([&]() { error_manager.error_at_pos("Error message", 1, 2, false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message (line 1, column 2)\nline 1\n ^\n");

    // Add another error message at a different position.
    stream_redirect.run([&]() { error_manager.error_at_pos("Error message 2", 2, 3, false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message (line 1, column 2)\nline 1\n ^\n");

    // Handle the error.
    error_manager.handle_error();
    CHECK_FALSE(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);

    // Add another error message at a different position.
    stream_redirect.clear();

    stream_redirect.run([&]() { error_manager.error_at_pos("Error message 3", 3, 4, false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 2);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(),
             "Error: Error message 3 (line 3, column 4)\nline 3\n   ^\n");
}

TEST_CASE("ErrorManager error at pos (no file lines)") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    // Add an error message at a position.
    stream_redirect.run([&]() { error_manager.error_at_pos("Error message", 1, 2, false); });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(), "Error: Error message (line 1, column 2)\n");
}

TEST_CASE("ErrorManager runtime error") {
    StreamRedirect stream_redirect;
    ErrorManager error_manager;

    // Set file lines
    std::vector<std::string> lines = {"line 1\n", "line 2\n", "line 3\n"};
    error_manager.set_file_lines(lines);

    // Add a runtime error message.
    stream_redirect.run([&]() {
        try {
            error_manager.runtime_error("Runtime error message", 1, 2);
        } catch (std::runtime_error &e) {
        }
    });
    CHECK(error_manager.check_error());
    CHECK_EQ(error_manager.get_error_count(), 1);
    CHECK_EQ(error_manager.get_status(), ErrorManager::BuildStatus::FAILURE);
    CHECK_EQ(stream_redirect.get_string(),
             "Runtime Error: Runtime error message (line 1, column 2)\nline 1\n ^\n");
}