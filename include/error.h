#ifndef SYNTHSCRIPT_ERROR_H
#define SYNTHSCRIPT_ERROR_H

#include "reader.h"
#include <string>

class Error {
public:
    static void error(const std::string& message, bool force_print=true);
    static void error_at_pos(const std::string& message, int line, int col, bool force_print=true);
    static void runtime_error(const std::string &message, int line, int col);

    static bool check_error();
    static void handle_error();
    static void print_build_status();
    static int get_error_count();
    static bool should_quit();

private:
    static int error_count;
    static bool errored;
};

#endif //SYNTHSCRIPT_ERROR_H
