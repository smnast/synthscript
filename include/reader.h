#ifndef SYNTHSCRIPT_READER_H
#define SYNTHSCRIPT_READER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Reader {
public:
    static bool file_exists(const std::string &path);
    static std::string read_file(const std::string &file_path);
    static void show_position(int line, int col);

private:
    static std::vector<std::string> file_lines;

    static std::string get_file_text(const std::string &file_path);
    static std::string clean_file(const std::string &file);
    static void prepare_file_lines(const std::string &file);
};

#endif // SYNTHSCRIPT_READER_H
