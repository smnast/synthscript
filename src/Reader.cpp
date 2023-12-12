#include "Reader.h"

bool Reader::fileExists(const std::string &path) {
    std::ifstream file(path);
    return file.good();
}

std::string Reader::getFileText(const std::string &filePath) {
    std::ifstream stream(filePath);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string Reader::cleanFile(const std::string &file) {
    std::string cleanedFile;
    bool commented = false;
    for (int i = 0; i < (int)file.size(); i++) {
        if (i < (int)file.size()-1 && file[i] == '/' && file[i+1] == '/') {
            commented = true;
        }
        if (file[i] == '\n') {
            commented = false;
        }
        if (!commented) {
            cleanedFile.push_back(file[i]);
        }
    }

    return cleanedFile;
}

std::string Reader::readFile(const std::string &filePath) {
    std::string fileContents = getFileText(filePath);
    std::string cleanedFile = cleanFile(fileContents);
    return cleanedFile;
}
