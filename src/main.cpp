#include <iostream>

#include "Reader.h"
#include "Lexer.h"
#include "Tokens.h"

void buildAndRun(const std::string &path) {
    std::string code = Reader::readFile(path);
    std::vector<Token> tokens = Lexer::parseTokens(code);
    for (auto t : tokens) {
        printf("%s\trow:%d\tcol:%d\n", tokenNames[t.tokenType].c_str(), t.lineNumber, t.columnNumber);
    }
}

void printUsage() {
    std::printf("Usage: sscript <path>\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2 && Reader::fileExists(argv[1])) {
        buildAndRun(argv[1]);
    } else {
        printUsage();
    }
}
