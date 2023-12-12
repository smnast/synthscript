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
    bool commented = false, multiline = false, stringLiteral = false;
    for (int i = 0; i < (int)file.size(); i++) {
        if (file[i] == '"' && (i == 0 || file[i-1] != '\\')) stringLiteral = !stringLiteral;
        if (!stringLiteral && file[i] == '#' && i < (int)file.size()-1 && file[i+1] == '#') {
            commented = !multiline;
            multiline = !multiline;
            i++;
            cleanedFile += "##";
            continue;
        } else if (!stringLiteral && file[i] == '#') {
            commented = true;
            cleanedFile.push_back('#');
        }
        if (file[i] == '\n' && !multiline) commented = false;
        if (!commented) {
            cleanedFile.push_back(file[i]);
        } else {
            if (file[i] == '\n') cleanedFile.push_back('\n');
            else cleanedFile.push_back(' ');
        }
    }

    return cleanedFile;
}

std::string Reader::readFile(const std::string &filePath) {
    std::string fileContents = getFileText(filePath);
    std::string cleanedFile = cleanFile(fileContents);
    return cleanedFile;
}
