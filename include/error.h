#ifndef SYNTHSCRIPT_ERROR_H
#define SYNTHSCRIPT_ERROR_H

#include <string>

class Error {
public:
    enum class BuildStatus {
        SUCCESS,
        FAILURE,
        WARNING
    };

    /**
     * @brief Print an error message if no other error has been encountered.
     * @param message The error message to print.
     * @param force_print Force the error message to be printed, even if another error has been
     * encountered.
     */
    static void error(const std::string &message, bool force_print = true);

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
    static void
    error_at_pos(const std::string &message, int line, int col, bool force_print = true);

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
    static void runtime_error(const std::string &message, int line, int col);

    /**
     * @brief Check if an error has been encountered and not yet handled.
     * @return True if there is an unhandled error, false otherwise.
     */
    static bool check_error();

    /**
     * @brief Handle the current error.
     */
    static void handle_error();

    /**
     * @brief Get the total error count.
     * @return Number of errors encountered during the build process.
     */
    static int get_error_count();

    /**
     * @brief Get the build status.
     * @return The build status.
     */
    static BuildStatus get_status();

private:
    /**
     * @brief The total number of errors during the build process.
     */
    static int error_count;

    /**
     * @brief If an error has been encountered and not yet handled.
     */
    static bool unhandled_error;
};

#endif // SYNTHSCRIPT_ERROR_H
