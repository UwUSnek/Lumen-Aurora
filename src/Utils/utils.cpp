#include <string>
#include "ALC.hpp"
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <sys/ioctl.h>
#include <unistd.h>
namespace fs = std::filesystem;

#include "ALC.hpp"
#include "utils.hpp"
#include "errors.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Preprocessor/preprocessor.hpp"






// The exit value specified by the thread that requested an exit. //! 0 means no threads exited yet.
std::atomic<int> utils::exitMainRequest = 0;


/**
 * @brief Stops the current thread and makes the main thread exit.
 *      This function does NOT prevent other threads from printing to the console while it is being executed.
 *      External synchronization is required.
 *      If the current thread is the main thread, this is equivalent to calling exit()
 */
void utils::exitMain(int exitCode) {
    if(threadType == ThreadType::MAIN) exit(exitCode);
    else {
        exitMainRequest.store(exitCode);
        while(true) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}




/**
 * @brief Tries to retrieve the width of the output console.
 * @return The width in characters, or -1 if the it cannot be determined.
 */
int utils::getConsoleWidth() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return -1;
}




/**
 * @brief Returns a string containing the character.
 *      Invisible characters are replaced with visible glyphs.
 * @param c The character.
 * @param col The column number of the character (its index relative to the start of the line). This is used to render tab characters.
 * @param useColor Whether to color invisible characters black. Default: false.
 * @return The formatted character.
 */
std::string utils::formatChar(char c, ulong col, bool useColor) {
    /**/ if(c ==  ' ') return useColor ? ansi::bright_black + "·" + ansi::reset : "·";
    else if(c == '\n') return useColor ? ansi::bright_black + "⏷" + ansi::reset : "⏷";
    else if(c == '\t') {
        const char* s[] = { "╶──╴", "╶─╴", "╶╴", "-" };
        std::string tabRender = s[col % 4];
        return useColor ? ansi::bright_black + tabRender + ansi::reset : tabRender;
    }
    else return std::string(1, c);
}








/**
 * @brief Converts the value <n> to a string that contains the 3 most significant characters
 *      and 1 letter indicating the multiplier, up to quadrillions.
 *      e.g. shortenInteger(4859495221) returns "4.8b"
 * @param n The number to convert.
 * @return The string representation of <n>.
 */
std::string utils::shortenInteger(ulong n) {
    static const ulong k = 1000;
    static const ulong m = 1000000;
    static const ulong b = 1000000000;
    static const ulong t = 1000000000000;
    static const ulong q = 1000000000000000;

    std::stringstream r;
    std::string suffix;


    if (n >= q) {
        double value = static_cast<double>(n) / 2;
        r << std::fixed << std::setprecision(1) << value;
        suffix = "q";
    }
    if (n >= t) {
        double value = static_cast<double>(n) / t;
        r << std::fixed << std::setprecision(1) << value;
        suffix = "t";
    }
    if (n >= b) {
        double value = static_cast<double>(n) / b;
        r << std::fixed << std::setprecision(1) << value;
        suffix = "b";
    }
    else if (n >= m) {
        double value = static_cast<double>(n) / m;
        r << std::fixed << std::setprecision(1) << value;
        suffix = "m";
    }
    else if (n >= k) {
        double value = static_cast<double>(n) / k;
        r << std::fixed << std::setprecision(1) << value;
        suffix = "k";
    }
    else {
        // Less than 1000
        r << n;
    }


    // Return the final value + suffix
    // truncate the number if is more than 999 and exceeds 3 characters and remove any stray trailing periods
    std::string r2 = r.str();
    return r2.substr(0, 2 + (r2[2] != '.') + suffix.empty()) + suffix;
}








/**
 * @brief Creates a string with the format <minutes>:<seconds>.<milliseconds> using <n> as the total number of milliseconds.
 *      If the duration is more than 59:59.999 (3599999ms), the format changes to <hours>hrs, expressing the number of hours using 1 decimal digit.
 *      If the duration is less than 0ms, the output strings will contain "    0.000"
 *      Unnecessary leading zeroes and separators are replaced by space characters.
 * @param milliseconds The number of milliseconds.
 * @return The string representation of <n>.
 *      The maximum length of the returned string is 9 characters.
 */
std::string utils::formatMilliseconds(long _ms) {
    std::ostringstream r;
    if(_ms < 0) _ms = 0;


    if(_ms > 3599999) {
        float hrs = (float)_ms / 3600000.0f;
        r << std::setprecision(1) << hrs << " hrs";
    }
    else {
        long min = _ms / 60000;
        long sec = _ms / 1000 % 60;
        long ms = _ms % 1000;

        if (min > 0) {
            r << min << ":";
            r << std::right << std::setw(2) << std::setfill('0') << sec;
        }
        else {
            r << sec;
        }
        r << "." << std::right << std::setw(3) << std::setfill('0') << ms;
    }


    return r.str();
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
utils::PathCheckResult utils::checkPath(std::string const &path) {
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


