#include "temp_file.h"
#include <fstream>
#include <iostream>
#include <utility>

TempFile::TempFile(const std::string &filename, const std::string &contents)
    : filename(std::move(filename)) {
    std::ofstream file(filename);
    file << contents;
    file.close();
}

TempFile::~TempFile() {
    std::remove(filename.c_str());
}
