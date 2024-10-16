#ifndef SYNTHSCRIPT_READER_H
#define SYNTHSCRIPT_READER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @class Reader
 * @brief Provides utility functions for reading and processing files.
 */
class Reader {
public:
    /**
     * @brief Checks if a file exists at the given path.
     * @param path The path to the file.
     * @return True if the file exists, otherwise false.
     */
    static bool file_exists(const std::string &path);

    /**
     * @brief Reads the contents of a file into a string.
     * @param file_path The path to the file.
     * @return The file contents as a string.
     */
    static std::string read_file(const std::string &file_path);

    /**
     * @brief Displays the position in the file based on the line and column number.
     * @param line The line number (1-based).
     * @param col The column number (1-based).
     */
    static void show_position(int line, int col);

private:
    /**
     * @brief The character used to denote comments in the file.
     */
    static const char comment_char;

    /**
     * @brief Retrieves the raw text from a file.
     * @param file_path The path to the file.
     * @return The raw file text.
     */
    static std::string get_file_text(const std::string &file_path);

    /**
     * @brief Cleans the file contents by removing comments.
     * @param file The raw file contents.
     * @return The cleaned file content as a string.
     */
    static std::string clean_file(const std::string &file);

    /**
     * @brief Stores the lines of the currently processed file.
     * Newlines are preserved within each line entry.
     */
    static std::vector<std::string> file_lines;

    /**
     * @brief Splits the file content into individual lines.
     * @param file The raw or cleaned file content.
     * @return A vector containing the lines of the file.
     */
    static std::vector<std::string> get_lines(const std::string &file);
};

#endif // SYNTHSCRIPT_READER_H
