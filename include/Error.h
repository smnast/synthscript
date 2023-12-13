#ifndef SYNTHSCRIPT_ERROR_H
#define SYNTHSCRIPT_ERROR_H

#include <string>
#include "Reader.h"

class Error {
public:
    static void error(const std::string& message, bool forcePrint=true);
    static void posError(const std::string& message, int line, int col, bool forcePrint=true);
    static bool checkError();
    static void handleError();
    static void printBuildStatus();
    static int getErrorCount();
    static bool shouldQuit();
private:
    static int errorCount;
    static bool errored;
};

#endif //SYNTHSCRIPT_ERROR_H
