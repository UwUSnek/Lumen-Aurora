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
#include "Preprocessor/ElmCoords.hpp"
#include "Preprocessor/preprocessor.hpp"






namespace utils {
    // The exit value specified by the thread that requested an exit. //! 0 means no threads exited yet.
    std::atomic<int> exitMainRequest = 0;


    /**
     * @brief Stops the current thread and makes the main thread exit.
     *      This function does NOT prevent other threads from printing to the console while it is being executed.
     *      External synchronization is required.
     *      If the current thread is the main thread, this is equivalent to calling exit()
     */
    void exitMain(int exitCode) {
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
    int getConsoleWidth() {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            return w.ws_col;
        }
        return -1;
    }




    /**
     * @brief Checks if a file path is a directory.
     * @return true if the path is a directory or a file that points to a directory, false if not.
     */
    bool isDir(std::string const &path) {
        return fs::is_directory(fs::is_symlink(path) ? fs::read_symlink(path).string() : path);
    }




    /**
     * @brief Prints a formatted line indicator and colors it black.
     *      The color is NOT reset after. The caller function will have to manually change it back.
    *      This function is NOT thread safe. Use a mutex to ensure other threads don't print at the same time.
     * @param n The number of the line. They start from 0, but the shown number is automatically increased by 1 to make it consistent with text exitors.
     */
    static inline void printLineNum(ulong n) {
        cerr << ansi::reset << ansi::bold_black << "\n" << std::right << std::setw(8) << n + 1 << " │ ";
    }


    /**
     * @brief Returns a string containing the character.
     *      Invisible characters are replaced with visible glyphs.
     * @param c The character.
     * @param useColor Whether to color invisible characters black. Default: false.
     * @param i The index of the character. This is used to render tab characters.
     * @return The formatted character.
     */
    std::string formatChar(char c, ulong i, bool useColor) {
        /**/ if(c ==  ' ') return useColor ? ansi::bright_black + "·" + ansi::reset : "·";
        else if(c == '\n') return useColor ? ansi::bright_black + "↓" + ansi::reset : "↓";
        else if(c == '\t') {
            const char* s[] = { "╶──╴", "╶─╴", "╶╴", "-" };
            std::string tabRender = s[i % 4];
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
    std::string shortenInteger(ulong n) {
        static const ulong k = 1000;
        static const ulong m = 1000000;
        static const ulong b = 1000000000;
        static const ulong t = 1000000000000;
        static const ulong q = 1000000000000000;

        std::stringstream r;
        std::string suffix;


        if (n >= 2) {
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
std::string formatMilliseconds(long _ms) {
    std::ostringstream r;
    if(_ms < 0) _ms = 0;


    if(_ms > 3599999) {
        //TODO test if this format actually works
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








    //FIXME    WHEN PRINTING LINES GENERATED BY A MACRO, MAKE IT VERY CLEAR THAT THEY WERE GENERATED
    //FIXME    ALSO WRITE WHICH MACRO CALL CREATED IT.
    //FIXME    maybe even write a raw version of the lines on the left and a replaced version of them on the right







    void printErrorGeneric(ErrorCode errorCode, std::string const &message) {
        consoleLock.lock();

        cerr << ansi::bold_red;
        cerr << "Error:";

        // Print the actual error after indenting it by 4 spaces
        cerr << ansi::bold_red << "\n    " << std::regex_replace(std::regex_replace(message, std::regex("\n"), "\n    "), std::regex("\033\\[0m"), ansi::bold_red) << "\n";

        // Stop the program
        consoleLock.unlock();
        exitMain(errorCode);
    }







    void printErrorCL(ErrorCode errorCode, cmd::ElmCoordsCL const &relPos, cmd::ElmCoordsCL const &errPos, std::string const &message, std::string const &fullCommand) {
        consoleLock.lock();

        cerr << ansi::bold_red;
        cerr << "Could not parse command:\n";


        // Print full command and highlight relevant section and error
        const char* lastColor;
        cerr << "    ";
        for(ulong i = 0; i < fullCommand.length(); ++i) {

            // Calculate current color based on the current character index and print it if it differs form the last one
            const char* curColor = ((i >= errPos.start && i <= errPos.end) ? ansi::bold_red : ((i >= relPos.start && i <= relPos.end) ? ansi::magenta : ansi::bright_black)).c_str();
            if(curColor != lastColor) {
                cerr << curColor;
                lastColor = curColor;
            }

            // Actually print the formatted character
            cerr << utils::formatChar(fullCommand[i], i);
        }


        // Print the actual error after indenting it by 4 spaces
        cerr << ansi::bold_red << "\n\n    " << std::regex_replace(std::regex_replace(message, std::regex("\n"), "\n    "), std::regex("\033\\[0m"), ansi::bold_red) << "\n";


        // Stop the program
        consoleLock.unlock();
        exitMain(errorCode);
    }






    /**
     * @brief Prints an error to stderr, specifying the error type.
     *      This function doesn't stop the program.
     * @param errorType The type of the error.
     * @param errPos The section of the code that caused the error.
     * @param message The error message. This can contain multiple lines.
     *      The error message will be colored red and displayed as bold. ansi::reset will reset to bold red.
     */
    void printError(ErrorCode errorCode, ErrType errType, ElmCoords const &errPos, std::string const &message) {
        printError(errorCode, errType, ElmCoords(), errPos, message);
    }



//FIXME SHOW MULTIPLE FILES IN THE CODE OUTPUT IF A SECTION IS SPLIT BETWEEN MULTIPLE SOURCE FILES
//FIXME SHOW MULTIPLE FILES IN THE CODE OUTPUT IF A SECTION IS SPLIT BETWEEN MULTIPLE SOURCE FILES


    //FIXME TRIM RELEVANT SECTION AND ERROR SECTION
    //FIXME remove all tabs, newlines and whitespaces before and after both
    //FIXME add a parameter to disable trimming

    /**
     * @brief Stop the program and prints an error to stderr, specifying the error type.
     * @param errorType The type of the error.
     * @param relPos The relevant section of the code.
     * @param errPos The section of the code that caused the error.
     * @param message The error message. This can contain multiple lines.
     *      The error message will be colored red and displayed as bold. ansi::reset will reset to bold red.
     */
    void printError(ErrorCode errorCode, ErrType errType, ElmCoords const &relPos, ElmCoords const &errPos, std::string const &message) {
        pre::sourceFilePathsLock.lock();
        std::string relFilePath = pre::sourceFilePaths[relPos.filePathIndex];
        std::string errFilePath = pre::sourceFilePaths[errPos.filePathIndex];
        pre::sourceFilePathsLock.unlock();

        consoleLock.lock();
        cerr << ansi::bold_red;

        // Print error type and location
        if(errType == ErrType::PREPROCESSOR) cerr << "Preprocessor";
        if(errType == ErrType::COMPILER)     cerr << "Compilation";
        cerr << " error:\n";


        // Find the line in the original file and calculate the starting index of the preceding line
        bool useRelevant = relFilePath.length();
        std::string s = readAndCheckFile(errFilePath);
        ulong curLine = useRelevant ? std::min(relPos.lineNum, errPos.lineNum) : errPos.lineNum;
        curLine -= !!curLine;
        ulong i       = useRelevant ? std::min(relPos.start,     errPos.start) : errPos.start;
        do --i; while(i != (ulong)-1L && s[i] != '\n'); if(i == (ulong)-1) i = 0;
        do --i; while(i != (ulong)-1L && s[i] != '\n'); if(i == (ulong)-1) i = 0;
        if(curLine > 0 && s[i] == '\n') ++i;


        // Print location
        ulong errHeight = std::count(s.c_str() + errPos.start, s.c_str() + errPos.end, '\n');
        if(errFilePath.length()) {
            cerr << "    File │ " << ansi::reset << fs::canonical(errFilePath) << ansi::bold_red << "\n";
            cerr << "    Line │ " << ansi::reset;
            if(errHeight == 0) cerr << errPos.lineNum + 1;
            else               cerr << "From " << errPos.lineNum + 1 << " to " << errPos.lineNum + errHeight + 1;
        }


        // Print all the interested lines and change color according to the indices of the relevant and offending sections
        cerr << "\n";
        printLineNum(curLine);
        ulong relHeight = std::count(s.c_str() + relPos.start, s.c_str() + relPos.end, '\n');
        ulong targetLineNum = std::max(errPos.lineNum + errHeight, relPos.lineNum + relHeight) + 1; //! No need to check useRelevant as its line is always 0 when unused
        const char* lastColor;
        for(; s[i] != '\0'; ++i) {

            // Calculate current color based on the current character index and print it if it differs form the last one
            const char* curColor = ((i >= errPos.start && i <= errPos.end) ? ansi::bold_red : ((i >= relPos.start && i <= relPos.end) ? ansi::magenta : ansi::bright_black)).c_str();
            if(curColor != lastColor) {
                cerr << curColor;
                lastColor = curColor;
            }

            // Actually print the formatted character and line number. Manually break if the current line exceeds the last line visible in the code output
            cerr << formatChar(s[i], i);
            if(s[i] == '\n') {
                ++curLine;
                if(curLine > targetLineNum) {
                    break;
                }
                printLineNum(curLine);
                cerr << lastColor;
            }
        }




        // Print the actual error after indenting it by 4 spaces
        cerr << ansi::bold_red << "\n\n    " << std::regex_replace(std::regex_replace(message, std::regex("\n"), "\n    "), std::regex("\033\\[0m"), ansi::bold_red) << "\n";


        // Stop the program
        consoleLock.unlock();
        exitMain(errorCode);
    }








    //FIXME CHANGE THIS FUNCTION SO THAT IT DOESN'T SHOW OTHER TYPE OF ERRORS AS PREPROCESSOR
    //FIXME CHANGE THIS FUNCTION SO THAT IT DOESN'T SHOW OTHER TYPE OF ERRORS AS PREPROCESSOR
    //FIXME CHANGE THIS FUNCTION SO THAT IT DOESN'T SHOW OTHER TYPE OF ERRORS AS PREPROCESSOR
    //FIXME CHANGE THIS FUNCTION SO THAT IT DOESN'T SHOW OTHER TYPE OF ERRORS AS PREPROCESSOR
    /**
     * @brief Reads a single source file and returns its contents as a string.
     *     Prints an error message if the file cannot be opened or doesn't exist.
     * @param fileName The path to the file.
     * @return The contents of the file as a string.
     */
    std::string readAndCheckFile(std::string const &fileName) {
        // Print an error if the file is a directory
        if(fs::is_directory(fileName)) {
            printError(
                ErrorCode::ERROR_PATH_IS_DIRECTORY,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(),
                "Could not open the specified file: \"" + fileName + "\": is a directory.\n" +
                "File path was interpreted as: " + ansi::white + "\"" + fs::canonical(fileName).string() + "\"" + ansi::reset + ".\n" +
                "Current working directory is: " + std::string(fs::current_path()) + "."
            );
        }

        // Create file stream and print an error if the file cannot be opened
        std::ifstream f(fileName, std::ios::binary);
        if(!f) {
            printError(
                ErrorCode::ERROR_PATH_CANNOT_OPEN,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(),
                "Could not open file \"" + fileName + "\": " + std::strerror(errno) + ".\n" +
                "Current working directory is: " + std::string(fs::current_path()) + "."
            );
        }


        // Read the contents of the file and return them as string
        return readFile(f);
    }








    /**
     * @brief Reads the contents of a file without performing any check or printing errors.
     * @param f The open file stream to read from.
     * @return The contents of the file as a string.
     */
    std::string readFile(std::ifstream &f) {
        f.seekg(0, std::ios::end);              // Move file pointer to the end of the file
        std::streamsize fileSize = f.tellg();   // Get file size in bytes
        f.seekg(0, std::ios::beg);              // Move file pointer back to index 0
        std::string r(fileSize, '\0');          // Preallocate the string
        f.read(&r[0], fileSize);                // Read the whole file at once
        return r;                               // Return the string value
    }
}


