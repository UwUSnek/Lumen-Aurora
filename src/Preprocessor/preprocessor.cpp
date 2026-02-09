
#include "preprocessor.hpp"
#include "ALC.hpp"
#include "Preprocessor/Phases/0-Include/includePhase.hpp"
#include "Preprocessor/Phases/0-Include/metadataGenerator.hpp"
#include "Preprocessor/Phases/1-LCTs/LCTsPhase.hpp"
#include "Preprocessor/Phases/2-Cleanup/cleanupPhase.hpp"
#include "Preprocessor/Phases/3-Macros/macroPhase.hpp"
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


    // Create pipes
    auto *r1 = new SegmentedCleanSource();
    auto *r2 = new SegmentedCleanSource();
    auto *r3 = new SegmentedCleanSource();
    auto *r4 = new SegmentedCleanSource();

    // Include all files
    //TODO add a command line option to disable waiting for all the includes.
    //TODO This option will make progress calculation less reliable and remove some features that need all the files to be known, but will speed up compilation
    startSubphaseAsync(Preprocessor_Includes, true, startIncludePhase, r0, r1);
    r1->str.awaitClose(mainCheckErrors);
    r1->meta.awaitClose(mainCheckErrors);

    // Start the other phases
    startSubphaseAsync(Preprocessor_LCT,      true, startLCTsPhase,    r1, r2);
    startSubphaseAsync(Preprocessor_Cleanup,  true, startCleanupPhase, r2, r3);
    startSubphaseAsync(Preprocessor_Macros,   true, startMacroPhase,   r3, r4);

    // Wait for the phases to finish and return the output buffer
    return r4;
}

//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.
//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.