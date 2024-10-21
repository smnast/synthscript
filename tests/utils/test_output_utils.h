#ifndef SYNTHSCRIPT_TESTOUTPUTUTILS_H
#define SYNTHSCRIPT_TESTOUTPUTUTILS_H

#include <iostream>
#include <sstream>
#include <functional>

/**
 * @class CoutRedirect
 * @brief Class to redirect cout to a string stream for testing purposes.
 */
class CoutRedirect {
public:
    /**
     * @brief Create a new CoutRedirect object.
     */
    CoutRedirect() : old_buf(std::cout.rdbuf()), ss() {}

    /**
     * @brief Destructor. Restores original cout.
     */
    ~CoutRedirect() {
        end();
    }

    /**
     * @brief Begin the redirection of cout to the string stream.
     */
    void begin() {
        std::cout.rdbuf(ss.rdbuf());
    }

    /**
     * @brief End the redirection of cout to the string stream.
     */
    void end() {
        std::cout.rdbuf(old_buf);
    }

    /**
     * @brief Get the string containing the cout output.
     * @return The string containing the cout output.
     */
    std::string get_string() const {
        return ss.str();
    }

    /**
     * @brief Clear the string stream.
     */
    void clear() {
        ss.str("");
        ss.clear();
    }

    /**
     * @brief Execute a function with cout redirected to a string stream.
     * @param func The function to execute.
     */
    void run(std::function<void()> func) {
        begin();
        func();
        end();
    }

private:
    /**
     * @brief The original cout buffer.
     */
    std::streambuf* old_buf;

    /**
     * @brief The string stream to redirect cout to.
     */
    std::ostringstream ss;
};

#endif // SYNTHSCRIPT_TESTOUTPUTUTILS_H
