#include <fstream>
#include <cstring>
#include <mutex>
#include "ALC.hpp"
#include "ErrorCode.hpp"
#include "FatalErrorException.hpp"
#include "Misc/CommentCounter.hpp"
#include "Misc/LctCounter.hpp"
#include "Misc/TextLiteralCounter.hpp"
#include "Misc/whitespaceCounter.hpp"
#include "Preprocessor/Phases/0-Include/metadataGenerator.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/Containers/StringPipe.hpp"
#include "Utils/errors.hpp"
#include "includePhase.hpp"
#include "pathSolver.hpp"








void pre::__internal_startIncludePhase(SegmentedCleanSource *b0, SegmentedCleanSource *r) {


    // Clean the code, saving the result in a temporary buffer. Skip validation checks
    //! This allows the text to be checked with standard functions and a way simpler logic.
    //! Performance overhead is negligible.
    ulong i  = 0; // b0 index
    ulong ii = 0; // b  index (clean)
    auto *b = new SegmentedCleanSource();
    auto skipped = std::vector<ulong>(b0->str.length(), 0); //! Oversized. Extra elements are simply not used. Initialize to all 0s
    while(b0->str[i].has_value()) {

        // Measure text to skip
        ulong        skipLen = misc::measureLct        (b0->str, i);  // Skip (and preserve) LCTs
        if(!skipLen) skipLen = misc::measureComment    (b0->str, i);  // Skip (and preserve) comments
        if(!skipLen) skipLen = misc::measureTextLiteral(b0->str, i);  // Skip (and preserve) literals
                                                                      // Skip (and preserve) macro definitions and invocations //FIXME
        // FIXME
        // #define name...\n     // Can include anything, including " and '
        // #name(...)            // Can include valid indentifiers, valid tokens, and `-limited parameters (which can contain anything)



        // Skip it and store the amount of skipped characters
        if(skipLen) {
            increaseLocalProgress(skipLen);
            skipped[ii] += skipLen;
            i += skipLen;
        }

        // Save character if not to be skipped
        else {
            b->str  += b0->str [i].value();
            b->meta += b0->meta[i].value();
            ++ii;
            ++i;
        }
    }

    // Close temporary pipe so the next step won't get stuck waiting for it forever
    b->str .closePipe();
    b->meta.closePipe();




    // Loop through the cleaned code
    i  = 0; // b0 index
    ii = 0; // b  index (clean)
    while(b->str[ii].has_value()) {
        std::string match;
        std::string filePathMatch;

        // If an include directive is detected, replace it with the preprocessed contents of the file
        parseIncludeStatementName(ii, b, match); //FIXME allow comments and LCTs inside of the include statement and path
        if(!match.empty()) {
            ulong j = ii + match.length();
            ElmCoords relevantCoords(b, ii, j - 1);

            // Skip whitespace if present
            j += misc::countWhitespace(b->str, j);

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

                        // Calculate the actual file path and open the file
                        std::string actualFilePath = resolveFilePath(rawIncludeFilePath, sourceFilePaths[b->meta[j]->f], relevantCoords, filePathCoords);
                        std::ifstream actualFile(actualFilePath);
                        //FIXME add a function that reads a file and saves it in a global array so they don't go out of scope


                        // Read and prepare code from the file
                        auto *fileCode = new SegmentedCleanSource();
                        totalFiles.fetch_add(1);
                        sourceFilePaths.push_back(actualFilePath);
                        auto newFilePathIndex = sourceFilePaths.size() - 1;
                        generateMetadata(utils::readFile(actualFile), fileCode, newFilePathIndex);


                        // Update phase progress data
                        increaseMaxProgress(Preprocessor_Includes, fileCode->str.length()); //! Self
                        increaseMaxProgress(Preprocessor_LCT,      fileCode->str.length());
                        increaseMaxProgress(Preprocessor_Cleanup,  fileCode->str.length());
                        increaseMaxProgress(Preprocessor_Macros,   fileCode->str.length());
                        increaseMaxProgress(Compiler_Tokenization, fileCode->str.length());


                        // Increase index (skip include and file path)
                        //! Update dirty buffer index, taking into account all of the stripped characters
                        const ulong old_i = i;
                        for(ulong jj = ii; jj < k; ++jj) {
                            i += 1 + skipped[jj];
                        }
                        ii = k;
                        increaseLocalProgress(i - old_i);
                        decreaseMaxProgress(Preprocessor_LCT,      i - old_i);
                        decreaseMaxProgress(Preprocessor_Cleanup,  i - old_i);
                        decreaseMaxProgress(Preprocessor_Macros,   i - old_i);
                        decreaseMaxProgress(Compiler_Tokenization, i - old_i);


                        // Append file data to r and process its includes recursively
                        __internal_startIncludePhase(fileCode, r);
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
                        "A file path must be specified.",
                        true //TODO recovery system. skip to the first token that makes sense
                    );
                }
            }

            // File path not found
            else {
                utils::printError(
                    ErrorCode::ERROR_PRE_NO_PATH,
                    utils::ErrType::PREPROCESSOR,
                    relevantCoords,
                    (!b->str[j].has_value()) ? relevantCoords : ElmCoords(b, j, j),
                    "Missing file path in include statement.\n"
                    "A valid file path was expected, but could not be found.",
                    true //TODO recovery system. skip to the first token that makes sense
                );
            }
        }


        // If not, copy normal characters and increase index counter
        //! Also appends all of the characters stripped from the dirty buffer
        else {
            const ulong old_i = i;
            for(ulong l = 0; l < 1 + skipped[ii]; ++l) {
                r->str  += b0->str [i].value();
                r->meta += b0->meta[i].value();
                ++i;
            }
            increaseLocalProgress(i - old_i);
            ++ii;
        }
    }
}








//! Manual regex because std doesn't support the custom pipe.
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








//! Manual regex because std doesn't support the custom pipe.
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
                "Standard module name is missing a closing \">\" character.", //! Copy incomplete string error message
                true //TODO recovery system. skip to the first token that makes sense
            );
        }

        char c = *b->str[i];
        if(c == '\n') {
            utils::printError(
                ErrorCode::ERROR_CMP_STRING_INCOMPLETE_n,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "Standard module name is missing a closing \">\" character.", //! Copy incomplete string error message
                true //TODO recovery system. skip to the first token that makes sense
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









void pre::startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

    // Try to execute the subphase
    try {
        __internal_startIncludePhase(b, r);
        r->str.closePipe();
        r->meta.closePipe();
    }

    // If errors occur, close the return pipes and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->str.closePipe();
        r->meta.closePipe();
        std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Preprocessing_A].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}