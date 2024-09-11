#include "ALC.hpp"
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>
namespace fs = std::filesystem;

#include "ALC.hpp"
#include "Command/command.hpp"
#include "Utils/utils.hpp"
#include "Utils/errors.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "includePhase.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Preprocessor/CleanupPhase/cleanupPhase.hpp"





namespace pre {
    void startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

        ulong i = 0; // The character index relative to the current file, not including included files
        while(b->str[i].has_value()) {
            std::string match;
            std::string filePathMatch;



            // Skip (and preserve) literals
            ulong literalLen = saveLiteral(b, i, r);
            if(literalLen) {
                i += literalLen;
                continue;
            }

            // Skip (and preserve) macro definitions and calls
            // FIXME
            // FIXME
            // FIXME


            // If an include directive is detected, replace it with the preprocessed contents of the file
            parseIncludeStatementName(i, b, match);
            if(!match.empty()) {
                ulong j = i + match.length();
                ElmCoords relevantCoords(b, i, j - 1);

                // Skip whitespace if present
                while(b->str[j] == ' ' || b->str[j] == '\t') ++j;

                // Detect specified file path
                parseIncludeStatementPath(j, b, filePathMatch);
                if(!filePathMatch.empty()) {
                    ulong k = j + filePathMatch.length();
                    ElmCoords filePathCoords(b, j, k - 1);

                    // File path is present
                    if(filePathMatch.length() > 2) {
                        //! Include path as written in the source file
                        std::string rawIncludeFilePath = filePathMatch.substr(1, filePathMatch.length() - 2);

                        // If the included file is a standard module
                        if(filePathMatch[0] == '<') {
                            //FIXME check name correctness and include the module
                            //TODO write this part without the check and copy parts to make the preprocessor work before standard modules are implemented
                            //TODO or maybe just check the name but don't include, since we already know what modules will be available
                        }

                        // If it is an actual file
                        else {

                            // Calculate the actual file path
                            std::string actualFilePath = resolveFilePath(rawIncludeFilePath, sourceFilePaths[b->meta[i]->f], relevantCoords, filePathCoords);

                            // Copy file contents and metadata
                            std::ifstream actualFile(actualFilePath);
                            std::string* fileContents = new std::string(utils::readFile(actualFile));
                            //FIXME add a function that reads a file and saves it in a global array so they don't go out of scope
                            actualFile.close();

                            totalFiles.fetch_add(1);
                            SegmentedCleanSource& preprocessedCode = loadSourceCode_loop(fileContents, actualFilePath);
                            preprocessedCode.str.awaitClose();
                            preprocessedCode.meta.awaitClose();
                            r->str  += *preprocessedCode.str.cpp();
                            r->meta += *preprocessedCode.meta.cpp();
                        }
                    }


                    // Empty string
                    else {
                        utils::printError(
                            ErrorCode::ERROR_PRE_EMPTY_PATH,
                            utils::ErrType::PREPROCESSOR,
                            relevantCoords,
                            filePathCoords,
                            "Empty file path in include statement.\n"
                            "A file path must be specified."
                        );
                    }


                    // Increase index (skip include and file path)
                    increaseLocalProgress(k - i);
                    i = k;
                }

                // File path not found
                else {
                    utils::printError(
                        ErrorCode::ERROR_PRE_NO_PATH,
                        utils::ErrType::PREPROCESSOR,
                        relevantCoords,
                        (!b->str[j].has_value()) ? relevantCoords : ElmCoords(b, j, j),
                        "Missing file path in include statement.\n"
                        "A valid file path was expected, but could not be found."
                    );
                }

            }


            // If not, copy normal characters and increase index counter
            else {
                r->str  += *b->str[i];
                r->meta += *b->meta[i];
                ++i;
            }
        }




        // return r;
        r->str.closePipe();
        r->meta.closePipe();
    }








    //! Manual regex because std doesn't support my custom pipe.
    //! Equivalent to checking /^#include[a-zA-Z0-9_]*[ \t]/ on b->str[i:]
    void parseIncludeStatementName(ulong index, pre::SegmentedCleanSource *b, std::string &match) {
        std::string tmp;
        ulong nameLen = sizeof("#include") - 1;
        ulong i = index + nameLen;
        if(b->str[i].has_value()) {
            if(!strncmp(b->str.cpp()->c_str() + index, "#include", nameLen)) {
                tmp += "#include";
            }
            else return;
        }
        else return;
        match = tmp;

        while(true) {
            char c = *b->str[i];
            if(std::isdigit(c) || std::isalpha(c) || c == '_') {
                match += c;
                ++i;
            }
            else break;
        }
    }








    //! Manual regex because std doesn't support my custom pipe.
    //! Equivalent to checking /^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>)/ on b->str[i:]
    void parseIncludeStatementPath(ulong index, pre::SegmentedCleanSource *b, std::string &filePathMatch) {
        std::string tmp;

        char type;
        ulong i = index;
        if(b->str[i].has_value()) {
            type = *b->str[i];
            if(type == '<' || type == '"') {
                tmp += type;
                ++i;
            }
            else return;
        }
        else return;

        char last = type;
        while(true) {
            if(!b->str[i].has_value()) {
                utils::printError(
                    ErrorCode::ERROR_STRING_INCOMPLETE_0,
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(b, index, i - 1),
                    ElmCoords(b, i - 1, i - 1),
                    "Standard module name is missing a closing \">\" character." //! Copy incomplete string error message
                );
            }

            char c = *b->str[i];
            if(c == '\n') {
                utils::printError(
                    ErrorCode::ERROR_STRING_INCOMPLETE_n,
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(b, index, i - 1),
                    ElmCoords(b, i - 1, i - 1),
                    "Standard module name is missing a closing \">\" character." //! Copy incomplete string error message
                );
            }
            else if(last != '\\' && c == (type == '<' ? '>' : '"')) {
                tmp += c;
                filePathMatch = tmp;
                return;
            }
            else {
                tmp += c;
                ++i;
                last = c;
            }
        }
    }








    /**
     * @brief Calculates the canonical path from the raw file path used in the include directive and prints an error if it's invalid or ambiguous.
     * @param rawFilePath The path that was found in the include directive, without any modification.
     * @param curFilePatht The path of the file that is currently being preprocessed.
     * @param relevantCoords The position of the relevant section.
     * @param filePathCoords The position of the section containing the include file path.
     * @return The canonical path of the specified file.
     */
    std::string resolveFilePath(std::string const &rawFilePath, std::string const &curFilePath, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords) {

        // If the path is an absolute path
        if(rawFilePath[0] == '/') {

            // Validate it and return its canonical version
            utils::PathCheckResult const &result = utils::checkPath(rawFilePath);
            return validateSelectedIncludePath(rawFilePath, result, relevantCoords, filePathCoords);
        }




        // If it is a relative path
        else {
            std::vector<std::pair<std::string, utils::PathCheckResult>> validPaths;
            std::vector<std::pair<std::string, utils::PathCheckResult>> invalidPaths;

            // Check current path
            std::string const &fullPath = fs::path(curFilePath).parent_path() / rawFilePath;
            utils::PathCheckResult &&result = utils::checkPath(fullPath);
            (result.exists ? validPaths : invalidPaths).push_back(std::pair<std::string, utils::PathCheckResult>(fullPath, result));

            // Check and categorize each include path
            for(std::string const &dir : cmd::options.includePaths) {
                std::string const &fullPath = dir + "/" + rawFilePath;
                utils::PathCheckResult const &result = utils::checkPath(fullPath);

                // Skip current file path if one equivalent to it already exists. If not, push it to the correct vector
                if(result.exists) {
                    std::string canonical = fs::canonical(fullPath);
                    for(int i = 0;; ++i) {
                        if(i < validPaths.size() && canonical == validPaths[i].first) {
                            break;
                        }
                        if(i >= validPaths.size()) { //! Checking -1 makes it overflow and loop forever
                            validPaths.push_back(std::pair<std::string, utils::PathCheckResult>(canonical, result));
                            break;
                        }
                    }
                }
                else invalidPaths.push_back(std::pair<std::string, utils::PathCheckResult>(fullPath, result));
            }


            // Print an error if no valid file path was found, listing all the paths that were tried
            if(validPaths.empty()) {
                std::string invalidPathsList = ansi::reset;
                for(ulong i = 0; i < invalidPaths.size(); ++i) {
                    invalidPathsList += "\n    " + std::to_string(i + 1) + ". \"" + ansi::white + invalidPaths[i].first + ansi::reset + "\"";
                }
                printError(
                    ErrorCode::ERROR_PRE_PATH_NOT_FOUND,
                    utils::ErrType::PREPROCESSOR,
                    relevantCoords,
                    filePathCoords,
                    "Could not open file \"" + rawFilePath + "\": no such file or directory.\n" +
                    "Paths tried: " + invalidPathsList
                );
            }

            // Print an error if more than one valid file path was found
            if(validPaths.size() > 1) {
                std::string validPathsList = ansi::reset;
                for(ulong i = 0; i < validPaths.size(); ++i) {
                    validPathsList += "\n    " + std::to_string(i + 1) + ". \"" + ansi::white + validPaths[i].first + ansi::reset + "\"";
                }
                printError(
                    ErrorCode::ERROR_PRE_PATH_AMBIGUOUS,
                    utils::ErrType::PREPROCESSOR,
                    relevantCoords,
                    filePathCoords,
                    "Ambiguous file path \"" + rawFilePath + "\".\n" +
                    "Files that match this path: " + validPathsList
                );
            }


            // If only one valid file path was found
            else {
                return validateSelectedIncludePath(validPaths[0].first, validPaths[0].second, relevantCoords, filePathCoords);
            }
        }


        //! Bogus return statement so GCC doesn't cry about it
        return "";
    }







    /**
     * @brief Prints an error if the path cannot be opened, is a directory or the compiler doesn't have read permissions on it.
     * @param filePath The path of the file.
     * @param checkResult The result of the check performed on the file.
     * @param relevantCoords The position of the relevant section.
     * @param filePathCoords The position of the section containing the include file path.
     * @return The canonical path of the file at <filePath>.
     */
    std::string validateSelectedIncludePath(std::string const &filePath, utils::PathCheckResult const &checkResult, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords) {

        // Print an error if the file doesn't exist
        if(!checkResult.exists) {
            printError(
                ErrorCode::ERROR_PRE_PATH_NOT_FOUND,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Could not open file \"" + filePath + "\": no such file or directory.\n" +
                "File path was interpreted as: \"" + ansi::white + filePath + ansi::reset + "\".\n" +
                "Make sure that the path is correct and the file exists."
            );
        }

        // Print an error if the file doesn't have read permission
        std::string actualFilePath = fs::canonical(filePath);
        if(!checkResult.canRead) {
            printError(
                ErrorCode::ERROR_PRE_PATH_NO_PERMISSION,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Could not open file \"" + filePath + "\": no read permission.\n" +
                "File path was interpreted as: \"" + ansi::white + actualFilePath + ansi::reset + "\"."
            );
        }

        // Print an error if the file is a directory
        if(checkResult.isDir) {
            printError(
                ErrorCode::ERROR_PRE_PATH_IS_DIRECTORY,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Could not include the specified path: \"" + filePath + "\" is a directory.\n" +
                "File path was interpreted as: \"" + ansi::white + actualFilePath + ansi::reset + "\"."
            );
        }

        // Reutrn canonical path
        return actualFilePath;
    }
}