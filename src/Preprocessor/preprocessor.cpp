#include "ALC.hpp"
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <thread>

#include "preprocessor.hpp"
#include "ALC.hpp"
#include "ElmCoords.hpp"
#include "Utils/utils.hpp"
#include "LCTsPhase/LCTsPhase.hpp"
#include "CleanupPhase/cleanupPhase.hpp"
#include "IncludePhase/includePhase.hpp"



namespace pre {



    // thread_local ulong threadId = -1;                     // The index of the local progress element that is managed by the current thread
    // //!                            ^ -1 is exclusively used to easily identify bugs. The value is initialized by pre::startPhaseThread


    /**
     * @brief The list of included source files in the order in which they were discovered.
     *      The source file passed through the command line is identified by the first element.
     *      All the paths saved in this vector are canonical paths.
     */
    std::vector<std::string> sourceFilePaths;
    std::mutex sourceFilePathsLock;







    // /**
    //  * @brief Creates a new element in the local progress list and sets the threadId to its index.
    //  *      This function MUST be called ONCE at the start of each phase by its own worker thread.
    //  *      Calling it from other threads, more than once or after starting the phase, WILL break progress detection.
    //  */
    // void initPhaseThread() {
    //     localProgress = new std::atomic<ulong>(0);

    //     localProgressArrayLock.lock();
    //     localProgressArray.push_back(localProgress);
    //     localProgressArrayLock.unlock();

    //     // pre::threadId = localProgress.size() - 1;
    // }






    // /**
    //  * @brief Creates a new element in the local progress list and sets the threadId to its index.
    //  *      This function MUST be called ONCE at the start of each phase by its own worker thread.
    //  *      Calling it from other threads, more than once or after starting the phase, WILL break progress detection.
    //  */
    // void stopPhaseThread() {
    // }
















    // SegmentedCleanSource& loadSourceCode(std::string &filePath) {
    //     std::string s = utils::readAndCheckFile(filePath);
    //     return loadSourceCode(s, filePath);
    // }


    static void _test(int n) {}

    /**
     * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
     * @param b The source code as a string.
     * @param filePath The path of the original source code file.
     * @return The contents of the source file as a SegmentedCleanSource.
     */
    SegmentedCleanSource& loadSourceCode(std::string const *s, std::string const &filePath) {
        sourceFilePathsLock.lock();
        sourceFilePaths.push_back(filePath); //TODO cache preprocessed files somewhere and add a function to chec for them before starting the preprocessor
        ulong pathIndex = sourceFilePaths.size() - 1;
        sourceFilePathsLock.unlock();


        //FIXME ^automatically fish up cached files if found. loop through them (for now)
        //FIXME                                               ^ use a hash map to save the paths of the preprocessed files

        //FIXME CHECK CIRCULAR DEPENDENCIES
        //FIXME SAFE INCLUDE STACK


        //TODO check if these can be moved to a global array
        //! ^ they are not 1-to-1. subphase threads can create new threads and start other phases,
        //!   which are separate from the main processing cluster
        SegmentedCleanSource *r1 = new SegmentedCleanSource(); //FIXME MOVE TO SUBPHASE DATA? maybe? idk
        SegmentedCleanSource *r2 = new SegmentedCleanSource(); //FIXME MOVE TO SUBPHASE DATA? maybe? idk
        SegmentedCleanSource *r3 = new SegmentedCleanSource(); //FIXME MOVE TO SUBPHASE DATA? maybe? idk

        // threadsLock.lock();
        // threads.emplace_back(std::thread(startLCTsPhase,     s, pathIndex, r1));
        // threads.emplace_back(std::thread(startCleanupPhase, r1,            r2));
        // threads.emplace_back(std::thread(startIncludePhase, r2,            r3));
        // threadsLock.unlock();
        startSubphaseAsync(PhaseID::PREPROCESSING, startLCTsPhase,     s, pathIndex, r1);
        startSubphaseAsync(PhaseID::PREPROCESSING, startCleanupPhase, r1,            r2);
        startSubphaseAsync(PhaseID::PREPROCESSING, startIncludePhase, r2,            r3);

        return *r3;
    }







    // // Add elements to output array. If include statements are found, parse the files and include them recursively
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r) {
    //     auto &e = r.elms;
    //     for(ulong i = 0; i < e.size();) {

    //         // Replace include statements with the specified file
    //         // if(e[i].type == SourceElmType::DIRECTIVE) {

    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // if(i < e.size() - 1 && e[i +1].type == SourceElmType::STRING) {

    //             // }
    //             // else {
    //             //     utils::printError(
    //             //         utils::ErrType::PREPROCESSOR,
    //             //         ElmCoords(e[i].meta),
    //             //         "Missing file path in include statement. A string literal was expected, but could not be found."
    //             //     );
    //             // }
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE

    //         //     continue;
    //         // }

    //         // Append anything else
    //         output.elms.push_back(e[i]);
    //         ++i;
    //     }
    // }









    //TODO add include backtrace with line and column number
    //TODO don't allow multi-byte characters inside of char literals

    //TODO check if multi-byte unicode characters can screw with the comment detection
}