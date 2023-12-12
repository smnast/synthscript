#ifndef SYNTHSCRIPT_ERROR_H
#define SYNTHSCRIPT_ERROR_H

#include <string>
#include "Reader.h"

class Error {
public:
    static void error(const std::string& message);
    static void posError(const std::string& message, int line, int col);
    static void printBuildStatus();
    static int getErrorCount();
    static bool shouldQuit();
private:
    static int errorCount;
};

#endif //SYNTHSCRIPT_ERROR_H
