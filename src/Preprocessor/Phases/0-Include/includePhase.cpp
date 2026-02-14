#include <fstream>
#include <sys/types.h>
#include "Main/ALC.hpp"
#include "Main/ErrorCode.hpp"
#include "Main/FatalErrorException.hpp"
#include "Preprocessor/Counters/CommentCounter.hpp"
#include "Preprocessor/Counters/LstCounter.hpp"
#include "Preprocessor/Counters/TextLiteralCounter.hpp"
#include "Preprocessor/Counters/whitespaceCounter.hpp"
#include "Preprocessor/Phases/0-Include/metadataGenerator.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Main/errors.hpp"
#include "includePhase.hpp"
#include "pathSolver.hpp"
#include "Preprocessor/preprocessor.hpp"








static constexpr const char* INCLUDE_TEXT = "#include";

//! Manual regex because std doesn't support the custom pipe.
//! Equivalent to checking /^#include[a-zA-Z0-9_]*[ \t]/ on b[i:]
static std::string parseIncludeStatementName(ulong index, pre::AnnotatedSource<false> &b) {
    if(b.strcmp(index, INCLUDE_TEXT)) {
        return INCLUDE_TEXT;
    }
    else return "";
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





template<typename f>
static ulong realignIndices(ulong& dirtyIndex, ulong& cleanIndex, const std::vector<ulong>& equivalentAmountVector, ulong jumpSize, f action) {
    const ulong dirtyIndexOld = dirtyIndex;
    for(ulong n = 0; n < jumpSize; ++n, ++cleanIndex) {
        for(ulong l = 0; l < equivalentAmountVector[cleanIndex]; ++l, ++dirtyIndex) {
            action(dirtyIndex);
        }
    }
    return dirtyIndex - dirtyIndexOld;
}








void pre::__internal_startIncludePhase(ptr<AnnotatedSource<false>> b0, ptr<AnnotatedSource<false>> r) {


    // Clean the code, saving the result in a temporary buffer. Skip validation checks
    //! This allows the text to be checked with standard functions and a way simpler logic.
    //! Performance overhead is negligible.
    ulong i  = 0; // b0 index
    ulong ii = 0; // b  index (clean)
    auto b = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_SMALL);
    auto equivalent = std::vector<ulong>(b0->length(), 0); //! Oversized. Extra elements are simply not used. Initialize to all 0s
    while((*b0)[i]) {
        ulong skipLen = 0;


        // Custom logic for LJTs (they need backwards lookup)
        //! Backwards lookup not actually implemented bc of design constraints.
        //! Instead, this checks for whitespace sequences and stores them if not removed by an adjacent LJT.
        //! Removes both the whitespace and LJT otherwise.
        const ulong whitespaceL = misc::countWhitespace(*b0, i);
        if(const ulong ljt = misc::measureLjt(*b0, i + whitespaceL); ljt) {
            const ulong whitespaceR = misc::countWhitespace(*b0, i + whitespaceL + ljt);
            skipLen = whitespaceL + ljt + whitespaceR;
            //! Set skipLen and let the if(skipLen) barch handle it
        }
        else if(whitespaceL) {
            for(ulong j = 0; j < whitespaceL; ++j) {
                ++equivalent[ii + j];
                *b += *(*b0)[i + j];
            }
            ii += whitespaceL;
            i  += whitespaceL;
            continue;
            //! Set data manually and go to the next iteration
        }


        // Measure text to skip
        if(!skipLen) skipLen = misc::measureLct        (*b0, i);  // Skip (and preserve) LCTs
        if(!skipLen) skipLen = misc::measureComment    (*b0, i);  // Skip (and preserve) comments
        if(!skipLen) skipLen = misc::measureTextLiteral(*b0, i);  // Skip (and preserve) literals
                                                                      // Skip (and preserve) macro definitions and invocations //FIXME
        // FIXME
        // #define name...\n     // Can include anything, including " and '
        // #name(...)            // Can include valid indentifiers, valid tokens, and `-limited parameters (which can contain anything)


        // Skip it and store the amount of skipped characters
        if(skipLen) {
            equivalent[ii] += skipLen;
            i += skipLen;
        }

        // Save character if not to be skipped
        else {
            *b += *(*b0)[i];
            ++equivalent[ii];
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
        std::string match = parseIncludeStatementName(ii, *b);
        if(!match.empty()) {
            ulong j = ii + match.length();
            ElmCoords relevantCoords(b, ii, j - 1);

            // Skip whitespace if present
            j += misc::countWhitespace(*b, j);

            // Detect specified file path
            if(const auto filePathMatch = parseIncludeStatementPath(j, *b); !filePathMatch.empty()) {
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
                        //FIXME add a function that reads a file and saves it in a global array so they can be reused (identified by their absolute path index)
                        //FIXME save raw files
                        //FIXME save preprocessed files
                        //FIXME save precompiled modules
                        //TODO save standard files & modules (by name, separate from files)


                        // Read and prepare code from the file
                        auto fileCode = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_SMALL);
                        totalFiles.fetch_add(1);
                        sourceFilePaths.push_back(actualFilePath);
                        auto newFilePathIndex = sourceFilePaths.size() - 1;
                        generateMetadata(utils::readFile(actualFile), fileCode, newFilePathIndex);


                        // Increase index (skip include and file path)
                        //! Update dirty buffer index, taking into account all of the stripped characters
                        ulong delta = realignIndices(i, ii, equivalent, k - ii, [](ulong){ /* Empty */ });


                        // Update phase progress data
                        increaseMaxProgress(fileCode->length(), P0_Includes, P1_LineSplicing, P2_Cleanup, P3_Macros, C0_Tokenization);
                        decreaseMaxProgress(delta,                           P1_LineSplicing, P2_Cleanup, P3_Macros, C0_Tokenization);
                        increaseLocalProgress(delta);


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
        //! Also append all of the characters stripped from the dirty buffer
        else {
            const ulong delta = realignIndices(i, ii, equivalent, 1, [&r, &b0](ulong dirtyIndex){
                *r += *(*b0)[dirtyIndex];
            });
            increaseLocalProgress(delta);
        }
    }


    // Process stripped characters at the end of the buffer
    //! Stripped sequences at the end of the buffer are not processed by the while loop as there is no next include statement to initiate the logic.
    //! This forcefully processes them. This is required in order to properly keep track of process and preserve all of the source code.
    if(i < b0->length()) {
        ulong delta = realignIndices(i, ii, equivalent, 1, [&r, &b0](ulong dirtyIndex){
            *r += *(*b0)[dirtyIndex];
        });
        increaseLocalProgress(delta);
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
        flagLocalError();
    }
}