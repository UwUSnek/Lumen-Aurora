#include <fstream>
#include <cstring>
#include <filesystem>
namespace fs = std::filesystem;

#include "ALC.hpp"
#include "Command/command.hpp"
#include "Utils/ansi.hpp"
#include "Utils/errors.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "includePhase.hpp"
#include "Preprocessor/CleanupPhase/cleanupPhase.hpp"
#include "pathSolver.hpp"





void pre::startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

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
                        SegmentedCleanSource *preprocessedCode = loadSourceCode_loop(fileContents, actualFilePath);
                        //! .awaitClose() is called by loadSourceCode_loop()
                        r->str  += *preprocessedCode->str.cpp();
                        r->meta += *preprocessedCode->meta.cpp();
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
void pre::parseIncludeStatementName(ulong index, pre::SegmentedCleanSource *b, std::string &match) {
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
void pre::parseIncludeStatementPath(ulong index, pre::SegmentedCleanSource *b, std::string &filePathMatch) {
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
                ErrorCode::ERROR_CMP_STRING_INCOMPLETE_0,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "Standard module name is missing a closing \">\" character." //! Copy incomplete string error message
            );
        }

        char c = *b->str[i];
        if(c == '\n') {
            utils::printError(
                ErrorCode::ERROR_CMP_STRING_INCOMPLETE_n,
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
