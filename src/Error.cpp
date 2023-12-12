#include "Error.h"

int Error::errorCount = 0;

void Error::error(const std::string &message) {
    std::printf("Error: %s\n", message.c_str());
    errorCount++;
}

void Error::posError(const std::string &message, int line, int col) {
    error(message);
    Reader::showPosition(line, col);
}

void Error::printBuildStatus() {
    std::string status = (errorCount > 0 ? "failed" : "succeeded");
    std::printf("=== Build %s with %d errors ===\n", status.c_str(), getErrorCount());
}

int Error::getErrorCount() {
    return errorCount;
}

bool Error::shouldQuit() {
    return errorCount > 0;
}
