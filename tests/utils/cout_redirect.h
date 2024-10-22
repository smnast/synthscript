#ifndef SYNTHSCRIPT_COUTREDIRECT_H
#define SYNTHSCRIPT_COUTREDIRECT_H

#include <functional>
#include <iostream>
#include <sstream>

/**
 * @class CoutRedirect
 * @brief Class to redirect cout to a string stream for testing purposes.
 */
class CoutRedirect {
public:
    /**
     * @brief Create a new CoutRedirect object.
     */
    CoutRedirect();

    /**
     * @brief Destructor. Restores original cout.
     */
    ~CoutRedirect();

    /**
     * @brief Begin the redirection of cout to the string stream.
     */
    void begin();

    /**
     * @brief End the redirection of cout to the string stream.
     */
    void end();

    /**
     * @brief Get the string containing the cout output.
     * @return The string containing the cout output.
     */
    std::string get_string() const;

    /**
     * @brief Clear the string stream.
     */
    void clear();

    /**
     * @brief Execute a function with cout redirected to a string stream.
     * @param func The function to execute.
     */
    void run(std::function<void()> func);

private:
    /**
     * @brief The original cout buffer.
     */
    std::streambuf *old_buf;

    /**
     * @brief The string stream to redirect cout to.
     */
    std::ostringstream ss;
};

#endif // SYNTHSCRIPT_COUTREDIRECT_H
