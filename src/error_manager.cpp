#include "error_manager.h"
#include "reader.h"
#include <iostream>

ErrorManager::ErrorManager() {
    error_count = 0;
    unhandled_error = false;
}

void ErrorManager::error(const std::string &message, bool force_print) {
    // Only print the error message if no other error has been encountered, unless forced to do so.
    if (!unhandled_error || force_print) {
        std::cout << "Error: " << message << std::endl;

        unhandled_error = true;
        error_count++;
    }
}

void ErrorManager::error_at_pos(const std::string &message, int line, int col, bool force_print) {
    // Only print the error message if no other error has been encountered, unless forced to do so.
    if (!unhandled_error || force_print) {
        std::string message_with_pos =
            message + " (line " + std::to_string(line) + ", column " + std::to_string(col) + ")";
        error(message_with_pos, true);
        show_position(line, col);
    }
}

void ErrorManager::runtime_error(const std::string &message, int line, int col) {
    std::cout << "Runtime Error: " << message << std::endl;
    show_position(line, col);
    exit(1);
}

void ErrorManager::show_position(int line, int col) {
    if (!file_lines.empty()) {
        // Show '^' under a specific position in the file
        std::string position = std::string(col - 1, ' ') + "^\n";
        std::cout << file_lines[line - 1] << position;
    }
}

bool ErrorManager::check_error() {
    return unhandled_error;
}

void ErrorManager::handle_error() {
    unhandled_error = false;
}

int ErrorManager::get_error_count() {
    return error_count;
}

ErrorManager::BuildStatus ErrorManager::get_status() {
    return (error_count > 0) ? BuildStatus::FAILURE : BuildStatus::SUCCESS;
}

void ErrorManager::set_file_lines(const std::vector<std::string> &lines) {
    file_lines = std::move(lines);
}
