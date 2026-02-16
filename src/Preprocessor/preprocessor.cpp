
#include "preprocessor.hpp"
#include "Main/ALC.hpp"
#include "Preprocessor/Phases/0-Include/includePhase.hpp"
#include "Preprocessor/Phases/0-Include/metadataGenerator.hpp"
#include "Preprocessor/Phases/1-LineSplicing/LineSplicingPhase.hpp"
#include "Preprocessor/Phases/2-Cleanup/cleanupPhase.hpp"
#include "Preprocessor/Phases/3-Macros/macroPhase.hpp"
#include "Preprocessor/AnnotatedSource.hpp"
#include <mutex>








/**
 * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
 * @param s The source code as a string.
 * @param filePath The path of the original source code file.
 * @return The contents of the source file as a AnnotatedSource.
 */
ptr<pre::AnnotatedSource<false>> pre::loadSourceCode(const std::string &s, const std::string &filePath) {
    ulong pathIndex;
    using enum PhaseID;
    {
        std::scoped_lock lock(sourceFilePathsLock);
        sourceFilePaths.push_back(filePath); //TODO cache preprocessed files somewhere and add a function to check for them before starting the preprocessor
        pathIndex = sourceFilePaths.size() - 1;
    }


    //FIXME ^automatically fish up cached files if found. loop through them (for now)
    //FIXME                                               ^ use a hash map to save the paths of the preprocessed files

    //FIXME CHECK CIRCULAR DEPENDENCIES

    //TODO SAVE INCLUDE STACK (replaces meta->f)
    //TODO use a map of include stacks (arrays of path indices)
    //TODO each path index represents the file of the stack's entry
    //TODO each character contains the index of the stack entry in the map. identical stacks use the same index. compare with the hash of the path index array


    // Load raw code of the root file
    auto r0 = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_SMALL);
    generateMetadata(s, r0, pathIndex);
    increaseMaxProgress(r0->length(), P0_Includes, P1_LineSplicing, P2_Cleanup, P3_Macros, C0_Tokenization);


    // Create pipes
    auto r1 = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_LARGE);
    auto r2 = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_LARGE);
    auto r3 = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_LARGE);
    auto r4 = newptr<AnnotatedSource<false>>(PREPROCESSOR_BUFFER_SIZE_LARGE);


    // Start phases and return the output buffer
    startSubphaseAsync(P0_Includes,     true, startIncludePhase,      r0, r1);
    // r1->awaitClose(mainCheckErrors); //TODO remove
    startSubphaseAsync(P1_LineSplicing, true, startLineSplicingPhase, r1, r2);
    startSubphaseAsync(P2_Cleanup,      true, startCleanupPhase,      r2, r3);
    startSubphaseAsync(P3_Macros,       true, startMacroPhase,        r3, r4);
    return r4;
}



// r1->str.awaitClose(mainCheckErrors);
// r1->meta.awaitClose(mainCheckErrors);
//TODO add a command line option to run one phase at a time.
//TODO This option will make progress calculation more reliable but significantly slow down compilation times.
//TODO It might help with debugging