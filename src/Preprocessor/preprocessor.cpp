
#include "ALC.hpp"
#include "ElmCoords.hpp"
#include "LCTsPhase/LCTsPhase.hpp"
#include "CleanupPhase/cleanupPhase.hpp"
#include "IncludePhase/includePhase.hpp"
#include "MacroPhase/macroPhase.hpp"








namespace pre {

    /**
     * @brief The part of loadSourceCode that does the recursive things
     */
    SegmentedCleanSource& loadSourceCode_loop(std::string const *s, std::string const &filePath) {
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

        startSubphaseAsync(PhaseID::PREPROCESSING, startLCTsPhase,     s, pathIndex, r1);
        startSubphaseAsync(PhaseID::PREPROCESSING, startCleanupPhase, r1,            r2);
        startSubphaseAsync(PhaseID::PREPROCESSING, startIncludePhase, r2,            r3);

        return *r3;
    }








    /**
     * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
     * @param b The source code as a string.
     * @param filePath The path of the original source code file.
     * @return The contents of the source file as a SegmentedCleanSource.
     */
    SegmentedCleanSource& loadSourceCode(std::string const *s, std::string const &filePath) {
        SegmentedCleanSource &r3 = loadSourceCode_loop(s, filePath);

        SegmentedCleanSource *r4 = new SegmentedCleanSource();
        startSubphaseAsync(PhaseID::PREPROCESSING, startMacroPhase, &r3, r4);
        return *r4;
    }
}