#include "reader.h"

std::vector<std::string> Reader::file_lines;

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
    bool commented = false, multi_line = false, string_literal = false;
    for (int i = 0; i < (int)file.size(); i++) {
        if (file[i] == '"' && (i == 0 || file[i - 1] != '\\')) {
            string_literal = !string_literal;
        }

        if (!string_literal && file[i] == '#' && i < (int)file.size() - 1 && file[i + 1] == '#') {
            commented = !multi_line;
            multi_line = !multi_line;
            i++;
            cleaned_file += "  ";
            continue;
        } else if (!string_literal && file[i] == '#') {
            commented = true;
            cleaned_file.push_back(' ');
        }

        if (file[i] == '\n' && !multi_line) {
            commented = false;
        }

        if (!commented) {
            cleaned_file.push_back(file[i]);
        } else {
            if (file[i] == '\n') {
                cleaned_file.push_back('\n');
            } else {
                cleaned_file.push_back(' ');
            }
        }
    }

    return cleaned_file;
}

std::string Reader::read_file(const std::string &file_path) {
    std::string file_contents = get_file_text(file_path);
    std::string cleaned_file = clean_file(file_contents);
    prepare_file_lines(cleaned_file);
    return cleaned_file;
}

void Reader::show_position(int line, int col) {
    std::string position = std::string(col - 1, ' ') + "^\n";
    std::printf("%s%s", file_lines[line - 1].c_str(), position.c_str());
}

void Reader::prepare_file_lines(const std::string &file) {
    std::string cur_row;
    for (char i : file) {
        cur_row.push_back(i);
        if (i == '\n') {
            file_lines.push_back(cur_row);
            cur_row.clear();
        }
    }
    if (!cur_row.empty()) {
        file_lines.push_back(cur_row);
    }
}
