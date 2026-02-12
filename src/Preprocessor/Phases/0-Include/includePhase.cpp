#include <fstream>
#include <cstring>
#include <mutex>
#include "Main/ALC.hpp"
#include "Main/ErrorCode.hpp"
#include "Main/FatalErrorException.hpp"
#include "Misc/CommentCounter.hpp"
#include "Misc/LctCounter.hpp"
#include "Misc/TextLiteralCounter.hpp"
#include "Misc/whitespaceCounter.hpp"
#include "Preprocessor/Phases/0-Include/metadataGenerator.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Main/errors.hpp"
#include "includePhase.hpp"
#include "Utils/ansi.hpp"
#include "pathSolver.hpp"
#include "Preprocessor/preprocessor.hpp"








static constexpr const char* INCLUDE_TEXT = "#include";

//! Manual regex because std doesn't support the custom pipe.
//! Equivalent to checking /^#include[a-zA-Z0-9_]*[ \t]/ on b[i:]
static std::string parseIncludeStatementName(ulong index, pre::AnnotatedSource<false> &b) {

    // Check include statement text
    std::string r;
    if(b.strcmp(index, INCLUDE_TEXT)) {
        r += INCLUDE_TEXT;
    }
    else return "";


    //FIXME what even is the point of this?
    //FIXME delete this if not needed. only check the actual statement name
    // Check and store specified include path
    ulong i = index + strlen(INCLUDE_TEXT);
    while(true) {
        char c = b[i]->c;
        if(std::isdigit(c) || std::isalpha(c) || c == '_') {
            r += c;
            ++i;
        }
        else break;
    }
    return r;
}








//! Manual regex because std doesn't support the custom pipe.
//! Equivalent to checking /^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>)/ on b[i:]
static std::string parseIncludeStatementPath(ulong index, pre::AnnotatedSource<false> &b) {
    std::string r;


    // Check if the first character is a < or " and store it. Return an empty string otherwise
    char type;
    ulong i = index;
    if(b[i]) {
        type = b[i]->c;
        if(type == '<' || type == '"') {
            r += type;
            ++i;
        }
        else return "";
    }
    else return "";


    // Loop through the path, print errors if the string ends unexpectedly. Compose and return the path otherwise
    char last = type;
    while(true) {
        if(!b[i]) {
            utils::printError(
                ErrorCode::ERROR_CMP_STRING_INCOMPLETE_0,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "Standard module name is missing a closing \">\" character.", //! Copy incomplete string error message
                true //TODO recovery system. skip to the first token that makes sense
            );
        }

        char c = b[i]->c;
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
            r += c;
            return r;
        }
        else {
            r += c;
            ++i;
            last = c;
        }
    }
    return r;
}








void pre::__internal_startIncludePhase(ptr<AnnotatedSource<false>> b0, ptr<AnnotatedSource<false>> r) {


    // Clean the code, saving the result in a temporary buffer. Skip validation checks
    //! This allows the text to be checked with standard functions and a way simpler logic.
    //! Performance overhead is negligible.
    ulong i  = 0; // b0 index
    ulong ii = 0; // b  index (clean)
    auto b = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_SMALL);
    auto skipped = std::vector<ulong>(b0->length(), 0); //! Oversized. Extra elements are simply not used. Initialize to all 0s
    while((*b0)[i]) {

        // Measure text to skip
        ulong        skipLen = misc::measureLct        (*b0, i);  // Skip (and preserve) LCTs
        if(!skipLen) skipLen = misc::measureComment    (*b0, i);  // Skip (and preserve) comments
        if(!skipLen) skipLen = misc::measureTextLiteral(*b0, i);  // Skip (and preserve) literals
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
            *b += *(*b0)[i];
            ++ii;
            ++i;
        }
    }

    // Close temporary pipe so the next step won't get stuck waiting for it forever
    b->closePipe();




    // Loop through the cleaned code
    i  = 0; // b0 index
    ii = 0; // b  index (clean)
    while((*b)[ii]) {

        // If an include directive is detected, replace it with the preprocessed contents of the file
        std::string match = parseIncludeStatementName(ii, *b); //FIXME allow comments and LCTs inside of the include statement and path
        if(!match.empty()) {
            ulong j = ii + match.length();
            ElmCoords relevantCoords(b, ii, j - 1);

            // Skip whitespace if present
            j += misc::countWhitespace(*b, j);

            // Detect specified file path
            std::string filePathMatch = parseIncludeStatementPath(j, *b);
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
                        using enum PhaseID;

                        // Calculate the actual file path and open the file
                        std::string actualFilePath = resolveFilePath(rawIncludeFilePath, sourceFilePaths[(*b)[j]->meta.f], relevantCoords, filePathCoords);
                        std::ifstream actualFile(actualFilePath);
                        //FIXME add a function that reads a file and saves it in a global array so they don't go out of scope


                        // Read and prepare code from the file
                        auto fileCode = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_SMALL);
                        totalFiles.fetch_add(1);
                        sourceFilePaths.push_back(actualFilePath);
                        auto newFilePathIndex = sourceFilePaths.size() - 1;
                        generateMetadata(utils::readFile(actualFile), fileCode, newFilePathIndex);


                        // Update phase progress data
                        increaseMaxProgress(Preprocessor_Includes, fileCode->length()); //! Self
                        increaseMaxProgress(Preprocessor_LCT,      fileCode->length());
                        increaseMaxProgress(Preprocessor_Cleanup,  fileCode->length());
                        increaseMaxProgress(Preprocessor_Macros,   fileCode->length());
                        increaseMaxProgress(Compiler_Tokenization, fileCode->length());


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
                    (!(*b)[j]) ? relevantCoords : ElmCoords(b, j, j),
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
                *r += *(*b0)[i];
                ++i;
            }
            increaseLocalProgress(i - old_i);
            ++ii;
        }
    }
}









void pre::startIncludePhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    // Try to execute the subphase
    try {
        __internal_startIncludePhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipes and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        std::scoped_lock lock(phaseDataArrayLock);
        phaseDataArray[(int)PhaseID::Preprocessor_Includes].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}