#include <fstream>
#include "Reader.h"

bool Reader::fileExists(const std::string &path) {
    std::ifstream file(path);
    return file.good();
}
