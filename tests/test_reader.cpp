#include "error_manager.h"
#include "reader.h"
#include "utils/cout_redirect.h"
#include "utils/temp_file.h"
#include <doctest/doctest.h>
#include <fstream>

TEST_CASE("Reader invalid file") {
    ErrorManager error_manager;
    std::string filename = "non_existent_file.txt";
    Reader reader(filename, &error_manager);
    CoutRedirect redirect;

    // Error when loading non-existent file
    redirect.run([&]() { reader.read_file(); });
    CHECK(error_manager.check_error());
    CHECK(redirect.get_string() == "Error: File '" + filename + "' does not exist\n");
}

TEST_CASE("Reader comments") {
    ErrorManager error_manager;
    CoutRedirect redirect;

    std::string comments =
        "# This is commented\n"
        "# Comment\n"
        "Single-line comments end after the line\n"
        "## Multi-line comments\n"
        "can span\n"
        "multiple lines ##\n"
        "They can also ## occur in the middle of a ## line\n"
        "But \"## no comments\" ## should occur in uncommented \"string literals ##\"\"\n"
        "Even \"# these ones\" # this works though\n"
        "##\n"
        "Multi-line comment\n"
        "# Comment within multi-line comment\n"
        "Still multi-line comment\n"
        "##\n"
        "# Comment ## Multi-line comment\n"
        "Not a comment\n"
        "## Tabs: \"	\", Spaces: \" \", and newlines: \"\n"
        "\" are preserved in comments ##\n";

    std::string comments_clean =
        "                   \n"
        "         \n"
        "Single-line comments end after the line\n"
        "                      \n"
        "        \n"
        "                 \n"
        "They can also                                line\n"
        "But \"## no comments\"                                                   \"\"\n"
        "Even \"# these ones\"                    \n"
        "  \n"
        "                  \n"
        "                                   \n"
        "                        \n"
        "  \n"
        "                               \n"
        "Not a comment\n"
        "          	                               \n"
        "                              \n";

    std::string filepath = "temp_comments.txt";
    TempFile temp_file(filepath, comments);

    // Load file without error
    Reader reader(filepath, &error_manager);
    std::string contents;
    redirect.run([&]() { contents = reader.read_file(); });
    CHECK_FALSE(error_manager.check_error());

    // Check that the contents are as expected
    CHECK(contents == comments_clean);
}

TEST_CASE("Reader file lines") {
    ErrorManager error_manager;
    CoutRedirect redirect;

    std::string file_contents = "line 1\n"
                                "line 2\n"
                                "line 3 # comments are in the lines too!\n"
                                "line 4 ## including\n"
                                "multi-line comments ##\n"
                                "line 6";

    std::vector<std::string> correct_lines = {
        "line 1\n",
        "line 2\n",
        "line 3 # comments are in the lines too!\n",
        "line 4 ## including\n",
        "multi-line comments ##\n",
        "line 6\n" // include a newline at EOF, even though it's not in the file
    };

    std::string filepath = "temp_lines.txt";
    TempFile temp_file(filepath, file_contents);

    // Load file into lines
    Reader reader(filepath, &error_manager);
    std::vector<std::string> lines;
    redirect.run([&]() {
        reader.read_file();
        lines = reader.get_lines();
    });
    CHECK_FALSE(error_manager.check_error());

    // Check that the lines are as expected
    CHECK(lines == correct_lines);
}
