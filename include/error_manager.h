#ifndef SYNTHSCRIPT_ERRORMANAGER_H
#define SYNTHSCRIPT_ERRORMANAGER_H

#include <string>
#include <vector>

class ErrorManager {
public:
    enum class BuildStatus { SUCCESS, FAILURE, WARNING };

    /**
     * @brief Create a new Error Manager object
     */
    ErrorManager();

    ~ErrorManager() = default;

    /**
     * @brief Print an error message if no other error has been encountered.
     * @param message The error message to print.
     * @param force_print Force the error message to be printed, even if another error has been
     * encountered.
     */
    void error(const std::string &message, bool force_print = true);

    /**
     * @brief Print an error message showing where the error occured.
     *
     * This function prints an error message showing the position in the file where
     * the error occurred. It only prints the message if no other errors have been
     * encountered unless forced to do so.
     *
     * @param message The error message to print.
     * @param line The line number where the error occurred.
     * @param col The column number where the error occurred.
     * @param force_print Force the error message to be printed, even if another error has been
     * encountered.
     */
    void error_at_pos(const std::string &message, int line, int col, bool force_print = true);

    /**
     * @brief Print a runtime error message showing where the error occured and exit the program.
     *
     * This function prints a runtime error message showing the position in the file where
     * the error occurred. The program will exit after printing the error.
     *
     * @param message The error message to print.
     * @param line The line number where the error occurred.
     * @param col The column number where the error occurred.
     */
    void runtime_error(const std::string &message, int line, int col);

    /**
     * @brief Check if an error has been encountered and not yet handled.
     * @return True if there is an unhandled error, false otherwise.
     */
    bool check_error();

    /**
     * @brief Handle the current error.
     */
    void handle_error();

    /**
     * @brief Get the total error count.
     * @return Number of errors encountered during the build process.
     */
    int get_error_count();

    /**
     * @brief Get the build status.
     * @return The build status.
     */
    BuildStatus get_status();

    /**
     * @brief Set the lines of the currently processed file.
     * @param lines The lines of the file.
     */
    void set_file_lines(const std::vector<std::string> &lines);

private:
    /**
     * @brief The total number of errors during the build process.
     */
    int error_count;

    /**
     * @brief If an error has been encountered and not yet handled.
     */
    bool unhandled_error;

    /**
     * @brief Stores the lines of the currently processed file.
     * Newlines are preserved within each line entry.
     */
    std::vector<std::string> file_lines;

    /**
     * @brief Displays the position in the file based on the line and column number.
     * @param line The line number (1-based).
     * @param col The column number (1-based).
     */
    void show_position(int line, int col);
};

#endif // SYNTHSCRIPT_ERRORMANAGER_H
