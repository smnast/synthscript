#include "error.h"

bool Error::errored = false;
int Error::errorCount = 0;

void Error::error(const std::string &message, bool forcePrint) {
    if (!errored || forcePrint) {
        std::printf("Error: %s\n", message.c_str());
        errorCount++;
    }

    errored = true;
}

void Error::posError(const std::string &message, int line, int col, bool forcePrint) {
    if (errored && !forcePrint) return;
    error(message, forcePrint);
    Reader::showPosition(line, col);
}

void Error::runtimeError(const std::string &message, int line, int col) {
    std::printf("Runtime Error: %s\n", message.c_str());
    Reader::showPosition(line, col);
    exit(1);
}

bool Error::checkError() {
    return errored;
}

void Error::handleError() {
    errored = false;
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
