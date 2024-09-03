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
    SegmentedCleanSource startIncludePhase(SegmentedCleanSource &b) {
        pre::initPhaseThread();
        pre::totalProgress.increaseTot(b.str.length());
        SegmentedCleanSource r;




        ulong i = 0; // The character index relative to the current file, not including included files
        while(i < b.str.length()) {
            std::smatch match;
            std::smatch filePathMatch;


            // If an include directive is detected, replace it with the preprocessed contents of the file
            if(std::regex_search(b.str.cbegin() + i, b.str.cend(), match, std::regex(R"(^#include(?![a-zA-Z0-9_])[ \t]*)"))) {
                ElmCoords relevantCoords(b, i, i + match[0].length() - 1);
                                                                                              //     ╭────── file ──────╮ ╭───── module ─────╮
                // Detect specified file path                                                 //     │ ╭────────────╮   │ │ ╭────────────╮   │
                if(std::regex_search(b.str.cbegin() + i + match[0].length(), b.str.cend(), filePathMatch, std::regex(R"(^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>))"))) {
                    ElmCoords filePathCoords(b, i + match[0].length(), i + match[0].length() + filePathMatch[0].length() - 1);


                    // File path is present
                    if(filePathMatch[0].length() > 2) {
                        //! Include path as written in the source file
                        std::string rawIncludeFilePath = filePathMatch[0].str().substr(1, filePathMatch[0].length() - 2);

                        // If the included file is a standard module
                        if(filePathMatch[0].str()[0] == '<') {
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
                            else adjustedIncludeFilePath = fs::path(sourceFilePaths[b.meta[i].f]).parent_path() / rawIncludeFilePath;


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


                            // Copy file contents and segments
                            std::string fileContents = utils::readFile(includeFile);
                            SegmentedCleanSource preprocessedCode = loadSourceCode(fileContents, canonicalIncludeFilePath); //FIXME run concurrently
                            r.str += preprocessedCode.str;

                            // Push all the segments from the included file
                            for(ulong j = 0; j < preprocessedCode.meta.size(); ++j) {
                                r.meta.push_back(preprocessedCode.meta[j]);
                            }
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
                    ulong fullLen = match[0].length() + filePathMatch[0].length();
                    i += fullLen;
                    pre::increaseLocalProgress(fullLen);
                }

                // File path not found
                else {
                    utils::printError(
                        ErrorCode::ERROR_PRE_NO_PATH,
                        utils::ErrType::PREPROCESSOR,
                        relevantCoords,
                        b.str[i + match[0].length() + filePathMatch[0].length()] == '\0' ? relevantCoords : ElmCoords(b, i, i),
                        "Missing file path in include statement.\n"
                        "A valid file path was expected, but could not be found."
                    );
                }

            }


            // If not, copy normal characters and increase index counter
            else {
                r.str += b.str[i];
                r.meta.push_back(b.meta[i]);
                ++i;
                pre::increaseLocalProgress(1);
            }
        }




        return r;
    }
}