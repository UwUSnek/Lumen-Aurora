
#include "preprocessor.hpp"
#include "ALC.hpp"
#include "ElmCoords.hpp"
#include "LCTsPhase/LCTsPhase.hpp"
#include "CleanupPhase/cleanupPhase.hpp"
#include "IncludePhase/includePhase.hpp"
#include "MacroPhase/macroPhase.hpp"








/**
 * @brief The part of loadSourceCode that does the recursive things
 */
pre::SegmentedCleanSource* pre::loadSourceCode_loop(std::string const *s, std::string const &filePath) {
    sourceFilePathsLock.lock();
    sourceFilePaths.push_back(filePath); //TODO cache preprocessed files somewhere and add a function to check for them before starting the preprocessor
    ulong pathIndex = sourceFilePaths.size() - 1;
    sourceFilePathsLock.unlock();


    //FIXME ^automatically fish up cached files if found. loop through them (for now)
    //FIXME                                               ^ use a hash map to save the paths of the preprocessed files

    //FIXME CHECK CIRCULAR DEPENDENCIES
    //FIXME SAVE INCLUDE STACK


    SegmentedCleanSource *r1 = new SegmentedCleanSource();
    SegmentedCleanSource *r2 = new SegmentedCleanSource();
    SegmentedCleanSource *r3 = new SegmentedCleanSource();

    startSubphaseAsync(PREPROCESSING, false, startLCTsPhase,     s, pathIndex, r1);
    startSubphaseAsync(PREPROCESSING, false, startCleanupPhase, r1,            r2);
    startSubphaseAsync(PREPROCESSING, false, startIncludePhase, r2,            r3);

    return r3;
}








/**
 * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
 * @param b The source code as a string.
 * @param filePath The path of the original source code file.
 * @return The contents of the source file as a SegmentedCleanSource.
 */
pre::SegmentedCleanSource* pre::loadSourceCode(std::string const *s, std::string const &filePath) {
    SegmentedCleanSource *r3 = loadSourceCode_loop(s, filePath);
    SegmentedCleanSource *r4 = new SegmentedCleanSource();


    r3-> str.awaitClose(); //! Wait for include phase to finish to improve the progress estimation //FIXME dont block the main thread but make the other phases wait for this one
    r3->meta.awaitClose(); //! Wait for include phase to finish to improve the progress estimation //FIXME dont block the main thread but make the other phases wait for this one
    startSubphaseAsync(PREPROCESSING, true, startMacroPhase, r3, r4);
    return r4;
}

//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.
//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.
//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.
//TODO FREE ALL THE SHARED BUFFERS WHEN NOT NEEDED ANYMORE.