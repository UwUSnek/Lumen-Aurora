#include <mutex>
#include <optional>
#include <regex>
#include <fstream>
#include <sstream>
#include <string_view>

#include "Main/errors.hpp"
#include "Utils/console.hpp"
#include "Utils/format.hpp"
#include "Utils/ansi.hpp"
#include "Main/ALC.hpp"








static const auto RESET_CODE_SEARCH_REGEX = std::regex("\033\\[0m");
using namespace console;

std::mutex  utils::errorMutex;
bool        utils::errorPresent = false;
std::string utils::errorMessage = "";


void utils::storeErrorMessage(const std::string_view &msg){
    std::scoped_lock lock(errorMutex);
    if(!errorPresent) {
        errorPresent = true;
        errorMessage = msg;
    }
}


std::optional<std::string> utils::getErrorMessage(){
    std::scoped_lock lock(errorMutex);
    if(errorPresent) {
        return errorMessage;
    }
    else{
        return std::nullopt;
    }
}








std::string utils::getErrTypeName(ErrType type) {
    switch(type) {
        case ErrType::COMPILER     : return "Compilation";
        case ErrType::PREPROCESSOR : return "Preprocessing";
        //TODO add the other types

        //! Bogus default case so the C++ compiler doesn't cry about it
        default: return "";
    }
}



/**
 * @brief Checks if a character is whitespace (either a space, \n or \t).
 * @return Whether a character is whitespace or not.
 */
static bool isWhitespace(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}




/**
 * @brief Resizes <coords> to remove any leading and trailing whitespace characters.
 * @param s The string <coords> refers to.
 *      Coordinates that point to after the string's last character are set to (ulong)-1.
 * @param coords The coordinates.
 * @return The trimmed coordinates.
 */
static inline ElmCoords trimCoords(const std::string &s, const ElmCoords &coords) {
    ulong end     = std::min(coords.end,   s.length());
    ulong start   = std::min(coords.start, s.length());
    ulong curLine = coords.lineNum;

    while(end > coords.start && isWhitespace(s[end])) {
        --end;
    }
    while(start < coords.end && isWhitespace(s[start])) {
        if(s[start] == '\n') ++curLine;
        ++start;
    }


    return ElmCoords(coords.filePathIndex, curLine, start, end, coords.overflow);
}




/**
 * @brief Resizes <coords> to remove any leading and trailing whitespace characters.
 * @param fullCommand The command string <coords> refers to.
 * @param coords The coordinates.
 * @return The trimmed coordinates.
 */
static inline cmd::ElmCoordsCL trimCoords(const std::string &fullCommand, const cmd::ElmCoordsCL &coords) {
    ulong end = coords.end;
    ulong start = coords.start;

    while(end >= coords.start && isWhitespace(fullCommand[end])) {
        --end;
    }
    while(start <= end && isWhitespace(fullCommand[start])) {
        ++start;
    }
    return cmd::ElmCoordsCL(start, end);
}








/**
 * @brief Creates a formatted line indicator and colors it black.
 *      The color is NOT reset after. The caller function will have to manually change it back.
 * @param n The number of the line. They start from 0, but the shown number is automatically increased by 1 to make it consistent with text exitors.
 * @return The formatted line number as a string.
 */
static inline std::string getLineNumStr(ulong n) {
    return std::format(
        "\n{}{}{:>8} │ ",
        ansi::reset, ansi::bold_black, n + 1
    );
}
















//TODO comment
void utils::printErrorGeneric(ErrorCode errorCode, const std::string &message, const bool fatal) {

    // Suppress error if other errors have occurred before it
    if(exitMainRequest.load()) return;


    storeErrorMessage(std::format(
        "{}Error:"
        "\n    {}"
        "\n",
        ansi::bold_red,
        std::regex_replace(
            std::regex_replace(message, std::regex("\n"), "\n    "),
            RESET_CODE_SEARCH_REGEX,
            ansi::bold_red
        )
    ));

    // Stop the program if needed
    if(fatal) exitMain((int)errorCode);
}







//TODO comment
void utils::printErrorCL(ErrorCode errorCode, cmd::ElmCoordsCL const &_relPos, cmd::ElmCoordsCL const &_errPos, const std::string &message, const bool fatal, const std::string &fullCommand) {
    std::stringstream r;

    // Suppress error if other errors have occurred before it
    if(exitMainRequest.load()) return;


    r << std::format(
        "{}Could not parse command:"
        "\n    ",
        ansi::bold_red
    );


    // Print full command and highlight relevant section and error
    const char* lastColor = nullptr;
    const cmd::ElmCoordsCL &relPos = trimCoords(fullCommand, _relPos);
    const cmd::ElmCoordsCL &errPos = trimCoords(fullCommand, _errPos);
    for(ulong i = 0; i < fullCommand.length(); ++i) {

        // Calculate current color based on the current character index and print it if it differs form the last one
        if(const char* curColor = (
            (i >= errPos.start && i <= errPos.end) ? ansi::bold_red : (
            (i >= relPos.start && i <= relPos.end) ? ansi::magenta :
            /**/                                     ansi::bright_black)
        ).c_str(); curColor != lastColor) {
            r << curColor;
            lastColor = curColor;
        }

        // Actually print the formatted character
        r << format::whitespace(fullCommand[i], i, true);
    }


    // Print the actual error after indenting it by 4 spaces
    r << std::format(
        "\n"
        "\n    {}{}"
        "\n",
        ansi::bold_red,
        std::regex_replace(
            std::regex_replace(message, std::regex("\n"), "\n    "),
            RESET_CODE_SEARCH_REGEX, ansi::bold_red
        )
    );


    storeErrorMessage(r.str());


    // Stop the program if needed
    if(fatal) exitMain((int)errorCode);
}








/**
 * @brief Prints an error to stderr, specifying the error type.
 * @param errorType The type of the error.
 * @param errPos The section of the code that caused the error.
 * @param message The error message. This can contain multiple lines.
 *      The error message will be colored red and displayed as bold. ansi::reset will reset to bold red.
 * @param fatal Whether the error was fatal or it can be recovered from. If true, calls exitMain().
 */
  void utils::printError(ErrorCode errorCode, ErrType errType, ElmCoords const &_errPos, const std::string &message, const bool fatal) {
    printError(errorCode, errType, _errPos, _errPos, message, fatal);
}








//FIXME SHOW MULTIPLE FILES IN THE CODE OUTPUT IF A SECTION IS SPLIT BETWEEN MULTIPLE SOURCE FILES
/**
 * @brief Prints an error to stderr, specifying the error type.
 * @param errorType The type of the error.
 * @param relPos The relevant section of the code.
 * @param errPos The section of the code that caused the error.
 * @param message The error message. This can contain multiple lines.
 *      The error message will be colored red and displayed as bold. ansi::reset will reset to bold red.
 * @param fatal Whether the error was fatal or it can be recovered from. If true, calls exitMain().
 */
void utils::printError(ErrorCode errorCode, ErrType errType, ElmCoords const &_relPos, ElmCoords const &_errPos, const std::string &message, const bool fatal) {
    std::stringstream r;


    // Suppress error if other errors have occurred before it
    if(exitMainRequest.load()) return;


    std::string relFilePath;
    std::string errFilePath;
    {
        std::scoped_lock lock(sourceFilePathsLock);
        relFilePath = sourceFilePaths[_relPos.filePathIndex];
        errFilePath = sourceFilePaths[_errPos.filePathIndex];
    }

    r << std::format(
        "{}{} error:",
        ansi::bold_red,
        getErrTypeName(errType)
    );



    // Check original file
    if(std::ifstream f(errFilePath); !f.is_open()) {

        // Print location
        if(errFilePath.length()) {
            r << std::format(
                "\n{}    File │ {}{}{} (source file unavailable){}"
                "\n{}    Line │ {}{}{}",
                ansi::bold_red, ansi::reset, errFilePath, ansi::bright_black, ansi::reset,
                ansi::bold_red, ansi::reset, _errPos.lineNum + 1, ansi::reset
            );
        }
    }
    else {

        // Find the line in the original file and calculate the starting index of the preceding line
        std::string s = readFile(f); //TODO use cached raw file
        f.close();
        ElmCoords const &relPos = trimCoords(s, _relPos);
        ElmCoords const &errPos = trimCoords(s, _errPos);
        ulong curLine = std::min(relPos.lineNum, errPos.lineNum); curLine -= !!curLine; //NOSONAR
        ulong i       = std::min(relPos.start,   errPos.start);
        do --i; while(i != (ulong)-1L && s[i] != '\n'); if(i == (ulong)-1) i = 0;
        do --i; while(i != (ulong)-1L && s[i] != '\n'); if(i == (ulong)-1) i = 0;
        if(curLine > 0 && s[i] == '\n') ++i;


        // Print location
        ulong errHeight = (ulong)std::count(s.c_str() + errPos.start, s.c_str() + errPos.end, '\n');
        if(errFilePath.length()) {
            r << std::format(
                "\n{}    File │ {}{}{}"
                "\n{}    Line │ {}{}{}",
                ansi::bold_red, ansi::reset, errFilePath, ansi::reset,
                ansi::bold_red, ansi::reset,
                errHeight == 0
                    ? std::format("{}", errPos.lineNum + 1)
                    : std::format("From {} to {}", errPos.lineNum + 1, errPos.lineNum + errHeight + 1),
                ansi::reset
            );
        }


        // Print all the interested lines and change color according to the indices of the relevant and offending sections
        r << "\n" << getLineNumStr(curLine);
        ulong relHeight = (ulong)std::count(s.c_str() + relPos.start, s.c_str() + relPos.end, '\n');
        ulong targetLineNum = std::max(errPos.lineNum + errHeight, relPos.lineNum + relHeight) + 1; //! No need to check useRelevant as its line is always 0 when unused
        const char* lastColor = nullptr;
        ulong col = 0;
        bool overflowed = false;
        for(;; ++i) {


            // Calculate current color based on the current character index and print it if it differs form the last one
            if(errPos.overflow) {
                const char* curColor = ((i >= relPos.start && i <= relPos.end) ? ansi::magenta : ansi::bright_black).c_str();
                if(curColor != lastColor) {
                    r << curColor;
                    lastColor = curColor;
                }

                // Print (missing code indicator) if needed
                if(!overflowed && i > errPos.end) {
                    overflowed = true;
                    r << ansi::bold_bright_red << " ﹏" << curColor;
                }
            }
            else {
                const char* curColor = ((i >= errPos.start && i <= errPos.end) ? ansi::bold_red : ((i >= relPos.start && i <= relPos.end) ? ansi::magenta : ansi::bright_black)).c_str();
                if(curColor != lastColor) {
                    r << curColor;
                    lastColor = curColor;
                }
            }


            // Actually print the formatted character and line number. Manually break if the current line exceeds the last line visible in the code output
            if(s[i] != '\0') {
                r << format::whitespace(s[i], col, false);
                ++col;
                if(s[i] == '\n') {
                    col = 0;
                    ++curLine;
                    if(curLine > targetLineNum) {
                        break;
                    }
                    r << getLineNumStr(curLine) << lastColor;
                }
            }
            else break;
        }
    }




    // Print the actual error after indenting it by 4 spaces
    r << std::format(
        "\n"
        "\n    {}{}"
        "\n",
        ansi::bold_red,
        std::regex_replace(
            std::regex_replace(message, std::regex("\n"), "\n    "),
            RESET_CODE_SEARCH_REGEX,
            ansi::bold_red
        )
    );


    storeErrorMessage(r.str());


    // Stop the program if needed
    if(fatal) exitMain((int)errorCode);
}