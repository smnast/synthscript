#ifndef SYNTHSCRIPT_READER_H
#define SYNTHSCRIPT_READER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Reader {
public:
    static bool fileExists(const std::string &path);
    static std::string readFile(const std::string &filePath);
    static void showPosition(int line, int col);
private:
    static std::vector<std::string> fileLines;
    static std::string getFileText(const std::string &filePath);
    static std::string cleanFile(const std::string &file);
    static void prepareFileLines(const std::string &file);
};

#endif //SYNTHSCRIPT_READER_H
