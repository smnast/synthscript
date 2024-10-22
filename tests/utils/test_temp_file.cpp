#include "temp_file.h"
#include <doctest/doctest.h>
#include <filesystem>
#include <fstream>

TEST_CASE("TempFile") {
    std::string filename = "test_file.txt";
    std::string contents = "This is a test file\n"
                           "with two lines.\n";
    
    // Scope with temp_file
    {
        TempFile temp_file(filename, contents);

        // TempFile creates a file
        bool file_exists = std::filesystem::exists(filename);
        CHECK(file_exists);

        // TempFile contains the correct contents
        std::ifstream file(filename);
        std::string line;
        std::getline(file, line);
        CHECK(line == "This is a test file");
        std::getline(file, line);
        CHECK(line == "with two lines.");
    } // temp_file is deleted here
    
    // TempFile deletes the file
    bool file_exists = std::filesystem::exists(filename);
    CHECK_FALSE(file_exists);
}
