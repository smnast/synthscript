#include "error.h"

bool Error::errored = false;
int Error::errorCount = 0;

void Error::error(const std::string &message, bool forcePrint) {
    if (!errored || forcePrint) {
        std::printf("Error: %s\n", message.c_str());
        errorCount++;
    }

    errored = true;
}

void Error::error_at_pos(const std::string &message, int line, int col, bool forcePrint) {
    if (errored && !forcePrint) return;
    error(message, forcePrint);
    Reader::show_position(line, col);
}

void Error::runtime_error(const std::string &message, int line, int col) {
    std::printf("Runtime Error: %s\n", message.c_str());
    Reader::show_position(line, col);
    exit(1);
}

bool Error::check_error() {
    return errored;
}

void Error::handle_error() {
    errored = false;
}

void Error::print_build_status() {
    std::string status = (errorCount > 0 ? "failed" : "succeeded");
    std::printf("=== Build %s with %d errors ===\n", status.c_str(), get_error_count());
}

int Error::get_error_count() {
    return errorCount;
}

bool Error::should_quit() {
    return errorCount > 0;
}
