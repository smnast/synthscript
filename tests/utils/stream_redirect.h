#ifndef SYNTHSCRIPT_STREAMREDIRECT_H
#define SYNTHSCRIPT_STREAMREDIRECT_H

#include <functional>
#include <iostream>
#include <sstream>

/**
 * @class StreamRedirect
 * @brief Class to redirect cin and cout to a string stream for testing purposes.
 */
class StreamRedirect {
public:
    /**
     * @brief Create a new StreamRedirect object.
     * @param stream The stream to redirect.
     */
    StreamRedirect();

    /**
     * @brief Destructor. Restores original stream.
     */
    ~StreamRedirect();

    /**
     * @brief Begin the redirection to the string stream.
     */
    void begin();

    /**
     * @brief End the redirection to the string stream.
     */
    void end();

    /**
     * @brief Get the string containing the cout output.
     * @return The string containing the cout output.
     */
    std::string get_string() const;

    /**
     * @brief Input a string to the string stream.
     */
    void give_string(std::string str);

    /**
     * @brief Clear the string stream.
     */
    void clear();

    /**
     * @brief Execute a function with the redirection active.
     * @param func The function to execute.
     */
    void run(std::function<void()> func);

private:
    /**
     * @brief The original cout buffer.
     */
    std::streambuf *old_cout_buf;

    /**
     * @brief The original cin buffer.
     */
    std::streambuf *old_cin_buf;

    /**
     * @brief The string stream to redirect cout to.
     */
    std::ostringstream cout_ss;

    /**
     * @brief The string stream to redirect cin to.
     */
    std::istringstream cin_ss;
};

#endif // SYNTHSCRIPT_STREAMREDIRECT_H
