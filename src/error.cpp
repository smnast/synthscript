#include "error.h"

bool Error::errored = false;
int Error::error_count = 0;

void Error::error(const std::string &message, bool force_print) {
    if (!errored || force_print) {
        std::printf("Error: %s\n", message.c_str());

        errored = true;
        error_count++;
    }
}

void Error::error_at_pos(const std::string &message, int line, int col, bool force_print) {
    if (!errored || force_print) {
        error(message, force_print);
        Reader::show_position(line, col);
    }
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
    std::string status = (error_count > 0 ? "failed" : "succeeded");
    std::printf("=== Build %s with %d errors ===\n", status.c_str(), get_error_count());
}

int Error::get_error_count() {
    return error_count;
}

bool Error::should_quit() {
    return error_count > 0;
}
