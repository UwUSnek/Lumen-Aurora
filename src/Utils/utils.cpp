#include "utils.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <sys/ioctl.h>
#include <unistd.h>

namespace fs = std::filesystem;








/**
 * @brief Tries to retrieve the size of the output console.
 * @return The size in characters, or (-1, -1) if the it cannot be determined.
 */
winsize utils::getConsoleSize() {
    struct winsize w;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w;
    }
    return { (ushort)-1, (ushort)-1, (ushort)-1, (ushort)-1 };
}







/**
 * @brief Reads the contents of a file without performing any check or printing errors.
 * @param f The open file stream to read from.
 * @return The contents of the file as a string.
 */
std::string utils::readFile(std::ifstream &f) {
    f.seekg(0, std::ios::end);              // Move file pointer to the end of the file
    std::streamsize fileSize = f.tellg();   // Get file size in bytes
    f.seekg(0, std::ios::beg);              // Move file pointer back to index 0
    std::string r(fileSize, '\0');          // Preallocate the string
    f.read(&r[0], fileSize);                // Read the whole file at once
    return r;                               // Return the string value
}









/**
 * @brief Checks if a path exists, if it is a directory or a file and retrieves its permissions.
 * @param fileName The path to check.
 * @return The result object containing all the informations.
 */
utils::PathCheckResult utils::checkPath(const std::string &path) {
    PathCheckResult r;
    if(access(path.c_str(), F_OK) == 0) {
        r.exists = true;
        r.canRead  = access(path.c_str(), R_OK) == 0;
        r.canWrite = access(path.c_str(), W_OK) == 0;
        r.canExec  = access(path.c_str(), X_OK) == 0;
        r.isDir = fs::is_directory(path);
    }
    return r;
}


