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
        // pre::totalProgress.increaseTot(b.str.length());
        //FIXME count progress of deleted characters + all characters of last phase




        ulong i = 0; // The character index relative to the current file, not including included files
        while(b->str[i].has_value()) {
            std::string match;
            std::string filePathMatch;



            // Skip (and preserve) literals
            ulong literalLen = saveLiteral(b, i, r);
            if(literalLen) {
                i += literalLen;
                increaseLocalProgress(literalLen);
                continue;
            }

            // Skip (and preserve) macro definitions and calls
            // FIXME
            // FIXME
            // FIXME


            // If an include directive is detected, replace it with the preprocessed contents of the file
            parseIncludeStatementName(i, b, match);
            if(!match.empty()) {
                ElmCoords relevantCoords(b, i, i + match.length() - 1);

                // Detect specified file path
                parseIncludeStatementPath(i + match.length(), b, filePathMatch);
                if(!filePathMatch.empty()) {
                    ElmCoords filePathCoords(b, i + match.length(), i + match.length() + filePathMatch.length() - 1);


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
                            //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
                            //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
                            //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
                            // Calculate the actual file path
                            std::string actualFilePath = resolveFilePath(rawIncludeFilePath, sourceFilePaths[b->meta[i]->f], relevantCoords, filePathCoords);


                            // Copy file contents and metadata
                            std::ifstream actualFile(actualFilePath);
                            std::string* fileContents = new std::string(utils::readFile(actualFile)); //FIXME add a function that read a file and saves it in a global array so they don't go out of scope
                            actualFile.close();

                            totalFiles.fetch_add(1);
                            SegmentedCleanSource& preprocessedCode = loadSourceCode(fileContents, actualFilePath); //FIXME run concurrently
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
                    ulong fullLen = match.length() + filePathMatch.length();
                    i += fullLen;
                    increaseLocalProgress(fullLen);
                }

                // File path not found
                else {
                    utils::printError(
                        ErrorCode::ERROR_PRE_NO_PATH,
                        utils::ErrType::PREPROCESSOR,
                        relevantCoords,
                        (!b->str[i + match.length()].has_value()) ? relevantCoords : ElmCoords(b, i + match.length(), i + match.length()),
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
                increaseLocalProgress(1);
            }
        }




        // return r;
        r->str.closePipe();
        r->meta.closePipe();
    }








    //! Manual regex because std doesn't support my custom pipe.
    //! Equivalent to checking /^#include[a-zA-Z0-9_]*[ \t]/ on b->str[i:]
    void parseIncludeStatementName(ulong i, pre::SegmentedCleanSource *b, std::string &match) {
        std::string tmp;
        ulong nameLen = sizeof("#include") - 1;
        ulong j = i + nameLen;
        if(b->str[j].has_value()) {
            if(!strncmp(b->str.cpp()->c_str() + i, "#include", nameLen)) {
                tmp += "#include";
            }
            else return;
        }
        else return;
        match = tmp;

        while(true) {
            char c = *b->str[j];
            if(std::isdigit(c) || std::isalpha(c) || c == '_') {
                match += c;
                ++j;
            }
            else break;
        }

        while(true) {
            char c = *b->str[j];
            if(c == ' ' || c == '\t') {
                match += c;
                ++j;
            }
            else break;
        }
    }








    //! Manual regex because std doesn't support my custom pipe.
    //! Equivalent to checking /^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>)/ on b->str[i:]
    void parseIncludeStatementPath(ulong i, pre::SegmentedCleanSource *b, std::string &filePathMatch) {
        std::string tmp;

        char type;
        if(b->str[i].has_value()) {
            type = *b->str[i];
            if(type == '<' || type == '"') {
                tmp += type;
                ++i;
            }
            else return;
        }

        char last = type;
        while(true) {
            char c = *b->str[i];
            if(c == '\n' || c == '\0') return; // TODO maybe write a more detailed error
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
     * @brief //TODO
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
                (result.exists ? validPaths : invalidPaths).push_back(std::pair<std::string, utils::PathCheckResult>(fullPath, result));
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
                    validPathsList += "\n    " + std::to_string(i + 1) + ". \"" + ansi::white + fs::canonical(validPaths[i].first).string() + ansi::reset + "\"";
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