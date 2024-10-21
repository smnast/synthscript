#ifndef SYNTHSCRIPT_READER_H
#define SYNTHSCRIPT_READER_H

#include "error_manager.h"
#include <string>
#include <vector>

/**
 * @class Reader
 * @brief Provides utility functions for reading and processing files.
 */
class Reader {
public:
    /**
     * @brief Construct a new Reader object.
     * @param path The path to the file to read.
     * @param error_manager The error manager to use for error handling.
     *
     * @note
     * The reader does not take ownership of the error manager object.
     */
    Reader(const std::string &file_path, ErrorManager *error_manager);

    /**
     * @brief Reads the contents of a file into a string.
     * @param file_path The path to the file.
     * @return The file contents as a string.
     */
    std::string read_file();

    /**
     * @brief Get the file lines.
     * @return The file lines.
     */
    std::vector<std::string> get_lines();

private:
    /**
     * @brief The error manager to use for error handling.
     */
    ErrorManager *error_manager;

    /**
     * @brief The path to the file to read.
     */
    std::string file_path;

    /**
     * @brief The contents of the file.
     */
    std::string file_contents;

    /**
     * @brief The character used to denote comments in the file.
     */
    const char comment_char = '#';

    /**
     * @brief Checks if a file exists at the given path.
     * @param path The path to the file.
     * @return True if the file exists, otherwise false.
     */
    bool file_exists(const std::string &path);

    /**
     * @brief Retrieves the raw text from a file.
     * @param file_path The path to the file.
     * @return The raw file text.
     */
    std::string get_file_text(const std::string &file_path);

    /**
     * @brief Cleans the file contents by removing comments.
     * @param file The raw file contents.
     * @return The cleaned file content as a string.
     */
    std::string clean_file(const std::string &file);

    /**
     * @brief Splits the file content into individual lines.
     * @param file_contents The raw or cleaned file content.
     * @return A vector containing the lines of the file.
     */
    std::vector<std::string> get_lines(const std::string &file_contents);
};

#endif // SYNTHSCRIPT_READER_H
