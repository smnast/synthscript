#include "error.h"
#include "reader.h"
#include <iostream>

bool Error::unhandled_error = false;
int Error::error_count = 0;

void Error::error(const std::string &message, bool force_print) {
    // Only print the error message if no other error has been encountered, unless forced to do so.
    if (!unhandled_error || force_print) {
        std::cout << "Error: " << message << std::endl;

        unhandled_error = true;
        error_count++;
    }
}

void Error::error_at_pos(const std::string &message, int line, int col, bool force_print) {
    // Only print the error message if no other error has been encountered, unless forced to do so.
    if (!unhandled_error || force_print) {
        error(message, true);
        Reader::show_position(line, col);
    }
}

void Error::runtime_error(const std::string &message, int line, int col) {
    std::cout << "Runtime Error: " << message << std::endl;
    Reader::show_position(line, col);
    exit(1);
}

bool Error::check_error() {
    return unhandled_error;
}

void Error::handle_error() {
    unhandled_error = false;
}

int Error::get_error_count() {
    return error_count;
}

Error::BuildStatus Error::get_status() {
    return (error_count > 0) ? BuildStatus::FAILURE : BuildStatus::SUCCESS;
}
