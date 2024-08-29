#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "Preprocessor/preprocessor.hpp"
#include "includePhase.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Preprocessor/CleanupPhase/cleanupPhase.hpp"






namespace pre {
    SegmentedCleanSource startIncludePhase(SegmentedCleanSource &b) {
        SegmentedCleanSource r;




        ulong i = 0; // The character index relative to the current file, not including included files
        while(i < b.str.length()) {


            // If an include directive is detected, replace it with the preprocessed contents of the file
            std::smatch match;
            if(std::regex_search(b.str.cbegin() + i, b.str.cend(), match, std::regex(R"(^#include(?![a-zA-Z0-9_])[ \t]*)"))) {
                ElmCoords relevantCoords(b, i, i + match[0].length() - 1);
                i += match[0].length();

                // Detect specified file path                                                 //     ╭────── file ──────╮ ╭───── module ─────╮
                std::smatch filePathMatch;                                                    //     │ ╭────────────╮   │ │ ╭────────────╮   │
                if(std::regex_search(b.str.cbegin() + i, b.str.cend(), filePathMatch, std::regex(R"(^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>))"))) {
                    ElmCoords filePathCoords(b, i, i + filePathMatch[0].length() - 1);
                    i += filePathMatch[0].length();


                    // File path is present
                    if(filePathMatch[0].length() > 2) {
                        std::string rawIncludeFilePath = filePathMatch[0].str().substr(1, filePathMatch[0].length() - 2);  //! Include path as written in the source file

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
                            std::filesystem::path adjustedIncludeFilePath = std::filesystem::canonical(sourceFilePaths[b.meta[i].f]).parent_path() / rawIncludeFilePath;    //! Include path relative to the file the include statement was used in
                            std::string canonicalIncludeFilePath;                                                                                           //! Canonical version of the adjusted file path. No changes if file was not found
                            try { canonicalIncludeFilePath = std::filesystem::canonical(adjustedIncludeFilePath).string(); }
                            catch(std::filesystem::filesystem_error e) { canonicalIncludeFilePath = adjustedIncludeFilePath.string(); }

                            // Print an error if the file is a directory
                            if(std::filesystem::is_directory(canonicalIncludeFilePath)) {
                                printError(
                                    utils::ErrType::PREPROCESSOR,
                                    relevantCoords,
                                    filePathCoords,
                                    "Could not include the specified path: \"" + rawIncludeFilePath + "\" is a directory.\n" +
                                    "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + "."
                                );
                            }

                            // Print an error if the file cannot be opened
                            std::ifstream includeFile(canonicalIncludeFilePath);
                            if(!includeFile) {
                                printError(
                                    utils::ErrType::PREPROCESSOR,
                                    relevantCoords,
                                    filePathCoords,
                                    "Could not open file \"" + rawIncludeFilePath + "\": " + std::strerror(errno) + ".\n" +
                                    "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + ".\n" +
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
                            utils::ErrType::PREPROCESSOR,
                            relevantCoords,
                            filePathCoords,
                            "Empty file path in include statement.\n"
                            "A file path must be specified."
                        );
                    }
                }

                // String literal not found
                else {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        relevantCoords,
                        b.str[i] == '\0' ? relevantCoords : ElmCoords(b, i, i),
                        "Missing file path in include statement.\n"
                        "A valid string literal was expected, but could not be found."
                    );
                }

            }


            // If not, copy normal characters and increase index counter
            else {
                r.str += b.str[i];
                r.meta.push_back(b.meta[i]);
                ++i;
            }
        }




        return r;
    }
}