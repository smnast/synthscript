#ifndef SYNTHSCRIPT_TEMPFILE_H
#define SYNTHSCRIPT_TEMPFILE_H

#include <string>

/**
 * @class TempFile
 * @brief Creates a temporary file that is deleted when the object is destroyed.
 * 
 * @note
 * This class assumes that the file operations will work.
 */
class TempFile {
public:
    /**
     * @brief Creates a temporary file with the given contents.
     * @param filename The name of the temporary file.
     * @param contents The contents of the temporary file.
     */
    TempFile(const std::string &filename, const std::string &contents);

    /**
     * @brief Deletes the temporary file.
     */
    ~TempFile();

private:
    /**
     * @brief Filename of the temporary file.
     */
    std::string filename;
};

#endif // SYNTHSCRIPT_TEMPFILE_H
