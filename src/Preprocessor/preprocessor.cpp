
#include "preprocessor.hpp"
#include "ALC.hpp"
#include "Preprocessor/Phases/0-Include/includePhase.hpp"
#include "Preprocessor/Phases/0-Include/metadataGenerator.hpp"
#include "Preprocessor/Phases/1-LCTs/LCTsPhase.hpp"
#include "Preprocessor/Phases/2-Cleanup/cleanupPhase.hpp"
#include "Preprocessor/Phases/3-Macros/macroPhase.hpp"
#include "Utils/Containers/StringPipe.hpp"
#include <functional>
#include <mutex>








/**
 * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
 * @param s The source code as a string.
 * @param filePath The path of the original source code file.
 * @return The contents of the source file as a SegmentedCleanSource.
 */
pre::SegmentedCleanSource* pre::loadSourceCode(const std::string *s, const std::string &filePath) {
    ulong pathIndex;
    {
        std::scoped_lock lock(sourceFilePathsLock);
        sourceFilePaths.push_back(filePath); //TODO cache preprocessed files somewhere and add a function to check for them before starting the preprocessor
        pathIndex = sourceFilePaths.size() - 1;
    }


    //FIXME ^automatically fish up cached files if found. loop through them (for now)
    //FIXME                                               ^ use a hash map to save the paths of the preprocessed files

    //FIXME CHECK CIRCULAR DEPENDENCIES
    //FIXME SAVE INCLUDE STACK

    // Load raw code of the root file
    auto *r0 = new SegmentedCleanSource();
    generateMetadata(*s, r0, pathIndex);


    // Start the loop subphases
    // startSubphaseAsync(Preprocessing, false, startLCTsPhase,     s, pathIndex, r1);
    // startSubphaseAsync(Preprocessing, false, startCleanupPhase, r1,            r2);
    // startSubphaseAsync(Preprocessing, false, startIncludePhase, r2,            r3);
    // startLCTsPhase(s, pathIndex, r1); //TODO

    // Start phases
    auto *r1 = new SegmentedCleanSource();
    auto *r2 = new SegmentedCleanSource();
    auto *r3 = new SegmentedCleanSource();
    auto *r4 = new SegmentedCleanSource();
    startSubphaseAsync(Preprocessor_Includes, true, startIncludePhase, r0, r1, pathIndex);
    startSubphaseAsync(Preprocessor_LCT,      true, startLCTsPhase,    r1, r2);
    startSubphaseAsync(Preprocessor_Cleanup,  true, startCleanupPhase, r2, r3);
    startSubphaseAsync(Preprocessor_Macros,   true, startMacroPhase,   r3, r4);


    // Wait for the phases to finish and return the output buffer
    r3-> str.awaitClose(mainCheckErrors); //! Wait for include phase to finish to improve the progress estimation //FIXME dont block the main thread but make the other phases wait for this one
    r3->meta.awaitClose(mainCheckErrors); //! Wait for include phase to finish to improve the progress estimation //FIXME dont block the main thread but make the other phases wait for this one
    return r4;


    // Load and merge all the files


    // Set the max progress of the compilation phase


    // Start the macro replacment phase and return the output
    // startSubphaseAsync(Preprocessing, true, startMacroPhase, r3, r4);
    // r4->str.awaitClose([](){}); //BUG this works, but it shouldn't be necessary
    // r4->meta.awaitClose([](){}); //BUG this works, but it shouldn't be necessary
    // return r4;
}

//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.
//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.