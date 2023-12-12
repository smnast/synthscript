#ifndef SYNTHSCRIPT_READER_H
#define SYNTHSCRIPT_READER_H

#include <string>
#include <fstream>
#include <sstream>

class Reader {
public:
    static bool fileExists(const std::string &path);
    static std::string readFile(const std::string &filePath);
private:
    static std::string getFileText(const std::string &filePath);
    static std::string cleanFile(const std::string &file);
};

#endif //SYNTHSCRIPT_READER_H
