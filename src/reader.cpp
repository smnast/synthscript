#include "reader.h"
#include <fstream>
#include <iostream>
#include <sstream>

const char comment_char = '#';

Reader::Reader(const std::string &file_path, ErrorManager *error_manager)
    : file_path(file_path), error_manager(error_manager) {}

std::string Reader::read_file() {
    if (!file_exists(file_path)) {
        error_manager->error("File '" + file_path + "' does not exist");
        return "";
    }

    file_contents = get_file_text(file_path);

    std::string cleaned_file = clean_file(file_contents);
    return cleaned_file;
}

std::vector<std::string> Reader::get_lines() {
    std::vector<std::string> file_lines = get_lines(file_contents);
    return file_lines;
}

bool Reader::file_exists(const std::string &path) {
    std::ifstream file(path);
    return file.good();
}

std::string Reader::get_file_text(const std::string &file_path) {
    std::ifstream stream(file_path);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string Reader::clean_file(const std::string &file) {
    std::string cleaned_file;
    bool commented = false, multi_line = false, in_string_literal = false;
    for (size_t i = 0; i < file.size(); i++) {
        // Update if we are in a string literal and not escaping it
        if (!commented && file[i] == '"' && (i == 0 || file[i - 1] != '\\')) {
            in_string_literal = !in_string_literal;
        }

        // Double comment character means start/end of a multi-line comment
        if (!(commented && !multi_line) && !in_string_literal && file[i] == comment_char &&
            i < (int)file.size() - 1 && file[i + 1] == comment_char) {
            commented = !multi_line;
            multi_line = !multi_line;
            cleaned_file += "  ";

            // Skip the next character as well
            i++;
            continue;
        }
        // Single comment character means start of a single-line comment
        else if (!(commented && multi_line) && !in_string_literal && file[i] == comment_char) {
            commented = true;
            cleaned_file.push_back(' ');
            continue;
        }

        // Single line comment ends at the end of the line
        if (file[i] == '\n' && !multi_line) {
            commented = false;
        }

        // Add the character to the cleaned file
        if (!commented) {
            cleaned_file.push_back(file[i]);
        } else {
            // White space is preserved in comments
            if (file[i] == '\n') {
                cleaned_file.push_back('\n');
            } else if (file[i] == '\t') {
                cleaned_file.push_back('\t');
            } else {
                cleaned_file.push_back(' ');
            }
        }
    }

    return cleaned_file;
}

std::vector<std::string> Reader::get_lines(const std::string &file_contents) {
    std::vector<std::string> lines;
    std::string cur_row;
    for (char i : file_contents) {
        cur_row.push_back(i);

        // New line
        if (i == '\n') {
            lines.push_back(cur_row);
            cur_row.clear();
        }
    }

    // Handle case where the last line does not end with a newline
    if (!cur_row.empty()) {
        cur_row.push_back('\n');
        lines.push_back(cur_row);
    }

    return lines;
}
