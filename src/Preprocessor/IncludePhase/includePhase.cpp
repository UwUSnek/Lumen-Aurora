#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>
namespace fs = std::filesystem;

#include "Preprocessor/preprocessor.hpp"
#include "includePhase.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Preprocessor/CleanupPhase/cleanupPhase.hpp"





namespace pre {
    void startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {
        pre::initPhaseThread();
        // pre::totalProgress.increaseTot(b.str.length());
        //FIXME count progress of deleted characters + all characters of last phase




        ulong i = 0; // The character index relative to the current file, not including included files
        while(b->str[i].has_value()) {
            // std::smatch match;           //TODO REMOVE
            // std::smatch filePathMatch;   //TODO REMOVE
            std::string match;
            std::string filePathMatch;



            // Skip (and preserve) literals
            ulong literalLen = saveLiteral(b, i, r);
            if(literalLen) {
                i += literalLen;
                pre::increaseLocalProgress(literalLen);
                continue;
            }

            // Skip (and preserve) macro definitions and calls
            // FIXME
            // FIXME
            // FIXME


            // If an include directive is detected, replace it with the preprocessed contents of the file
            // if(std::regex_search(b.str.cbegin() + i, b.str.cend(), match, std::regex(R"(^#include(?![a-zA-Z0-9_])[ \t]*)"))) { //TODO REMOVE
            parseIncludeStatementName(i, b, match);
            if(!match.empty()) {
                ElmCoords relevantCoords(b, i, i + match.length() - 1);
                                                                                              //     ╭────── file ──────╮ ╭───── module ─────╮
                // Detect specified file path                                                 //     │ ╭────────────╮   │ │ ╭────────────╮   │
                // if(std::regex_search(b.str.cbegin() + i + match[0].length(), b.str.cend(), filePathMatch, std::regex(R"(^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>))"))) { //TODO REMOVE

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

                            //! Include path relative to the file the include statement was used in. No changes if the raw path is an absolute path
                            fs::path adjustedIncludeFilePath;
                            if(rawIncludeFilePath[0] == '/') adjustedIncludeFilePath = rawIncludeFilePath;
                            else {
                                sourceFilePathsLock.lock();
                                adjustedIncludeFilePath = fs::path(sourceFilePaths[b->meta[i]->f]).parent_path() / rawIncludeFilePath;
                                sourceFilePathsLock.unlock();
                            }


                            //! Canonical version of the adjusted file path. No changes if file was not found
                            std::string canonicalIncludeFilePath;
                            try { canonicalIncludeFilePath = fs::canonical(adjustedIncludeFilePath).string(); }
                            catch(fs::filesystem_error e) { canonicalIncludeFilePath = adjustedIncludeFilePath.string(); }


                            // Print an error if the file is a directory
                            if(utils::isDir(canonicalIncludeFilePath)) {
                                printError(
                                    ErrorCode::ERROR_PRE_PATH_IS_DIRECTORY,
                                    utils::ErrType::PREPROCESSOR,
                                    relevantCoords,
                                    filePathCoords,
                                    "Could not include the specified path: \"" + rawIncludeFilePath + "\" is a directory.\n" +
                                    "File path was interpreted as: \"" + ansi::white + canonicalIncludeFilePath + ansi::reset + "\"."
                                );
                            }


                            // Print an error if the file cannot be opened
                            std::ifstream includeFile(canonicalIncludeFilePath);
                            if(!includeFile) {
                                printError(
                                    ErrorCode::ERROR_PRE_PATH_CANNOT_OPEN,
                                    utils::ErrType::PREPROCESSOR,
                                    relevantCoords,
                                    filePathCoords,
                                    "Could not open file \"" + rawIncludeFilePath + "\": " + std::strerror(errno) + ".\n" +
                                    "File path was interpreted as: \"" + ansi::white + canonicalIncludeFilePath + ansi::reset + "\".\n" +
                                    "Make sure that the path is correct and the compiler has read access to the file."
                                );
                            }


                            // Copy file contents and metadata
                            std::string* fileContents = new std::string(utils::readFile(includeFile)); //FIXME add a function that read a file and saves it in a global array so they don't go out of scope
                            SegmentedCleanSource& preprocessedCode = loadSourceCode(fileContents, canonicalIncludeFilePath); //FIXME run concurrently
                            preprocessedCode.str.awaitClose();
                            preprocessedCode.meta.awaitClose();
                            r->str  += *preprocessedCode.str.cpp();
                            r->meta += *preprocessedCode.meta.cpp();

                            // // Push all the segments from the included file
                            // for(ulong j = 0; j < preprocessedCode.meta.length(); ++j) {
                            //     r->meta += *preprocessedCode.meta[j];
                            // }
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
                    pre::increaseLocalProgress(fullLen);
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
                pre::increaseLocalProgress(1);
            }
        }




        // return r;
        r->str.closePipe();
        r->meta.closePipe();
        pre::stopPhaseThread();
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
}