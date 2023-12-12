#include <iostream>

#include "Reader.h"

void buildAndRun(const std::string &path) {

}

void printUsage() {
    std::cout << "Usage: sscript <path>" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc == 2 && Reader::fileExists(argv[1])) {
        buildAndRun(argv[1]);
    } else {
        printUsage();
    }
}
