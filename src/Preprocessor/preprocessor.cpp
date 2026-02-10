
#include "preprocessor.hpp"
#include "Main/ALC.hpp"
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
ptr<pre::SegmentedCleanSource> pre::loadSourceCode(const std::string &s, const std::string &filePath) {
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
    auto r0 = newptr<SegmentedCleanSource>();
    generateMetadata(s, r0, pathIndex);


    // Create pipes
    auto r1 = newptr<SegmentedCleanSource>();
    auto r2 = newptr<SegmentedCleanSource>();
    auto r3 = newptr<SegmentedCleanSource>();
    auto r4 = newptr<SegmentedCleanSource>();

    // Include all files
    startSubphaseAsync(PhaseID::Preprocessor_Includes, true, startIncludePhase, r0, r1);
    // r1->str.awaitClose(mainCheckErrors);
    // r1->meta.awaitClose(mainCheckErrors);
    //TODO add a command line option to run one phase at a time.
    //TODO This option will make progress calculation more reliable but significantly slow down compilation times.
    //TODO It might help with debugging

    // Start the other phases
    startSubphaseAsync(PhaseID::Preprocessor_LCT,      true, startLCTsPhase,    r1, r2);
    startSubphaseAsync(PhaseID::Preprocessor_Cleanup,  true, startCleanupPhase, r2, r3);
    startSubphaseAsync(PhaseID::Preprocessor_Macros,   true, startMacroPhase,   r3, r4);

    // Wait for the phases to finish and return the output buffer
    return r4;
}