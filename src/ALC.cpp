#include <iostream>
#include <vector>

#include "ALC.hpp"
#include "Utils/ansi.hpp"




//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs




//! Set by the main function
VersionNumber versionNumer;




// The exit value specified by the thread that requested an exit. //! 0 means no threads exited yet.
std::atomic<int> exitMainRequest(0);


/**
 * @brief Stops the current thread and makes the main thread exit.
 *      This function does NOT prevent other threads from printing to the console while it is being executed.
 *      External synchronization is required.
 *      If the current thread is the main thread, this is equivalent to calling exit()
 */
void exitMain(int exitCode) {
    if(threadType == ThreadType::MAIN) exit(exitCode);
    else {
        exitMainRequest.store(exitCode);
        while(true) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}










/**
 * @brief The list of included source files in the order in which they were discovered.
 *      The source file passed through the command line is identified by the first element.
 *      All the paths saved in this vector are canonical paths.
 */
std::vector<std::string> sourceFilePaths;
std::mutex sourceFilePathsLock;



std::atomic<ulong> activeThreads(0);
std::atomic<ulong> totalThreads(0);
std::atomic<ulong> totalFiles(0);
std::atomic<ulong> totalModules(0);

thread_local ThreadType threadType = ThreadType::UNKNOWN;








//FIXME detect all the files before processing them to make progress more reliable
// Becomes true after the output file gets written and closed
std::atomic<bool> isComplete(false);












// A mutex that controls the console output.
// This allows multiple threads to write multiple messages at once without being interrupted.
std::mutex __internal_consoleLock;




int __internal_cout_stream_t::overflow(int c) {
    if (c != EOF) {
        if (c == '\n') std::cout << "\033[K";
        std::cout.put(c);
    }
    return c;
}
__internal_cout_stream_t __internal_cout_streambuff;
std::ostream __internal_cout(&__internal_cout_streambuff);
__internal_cout_stream_t_wrapper cout;





int __internal_cerr_stream_t::overflow(int c) {
    if (c != EOF) {
        if (c == '\n') std::cerr << "\033[K";
        std::cerr.put(c);
    }
    return c;
}
__internal_cerr_stream_t __internal_cerr_streambuff;
std::ostream __internal_cerr(&__internal_cerr_streambuff);
__internal_cerr_stream_t_wrapper cerr;













std::string phaseIdTotring(PhaseID phaseId) {
    static const char* names[] = {
        #define X(e) #e,
        LIST_PHASE_ID
        #undef X
    };
    return names[phaseId];
}



PhaseData::PhaseData() :
    totalProgress(new DynamicProgressBar(0, ansi::bright_green, ansi::bright_black)),
    timeStart      (new std::atomic<long>(0)),
    timeEnd        (new std::atomic<long>(0)) {
}


std::vector<PhaseData> phaseDataArray;
std::mutex             phaseDataArrayLock;

std::vector<SubphaseData> subphaseDataArray;
std::mutex                subphaseDataArrayLock;

thread_local std::atomic<ulong>* localProgress = nullptr;
thread_local DynamicProgressBar*   maxProgress = nullptr;


/**
 * @brief Increases the local progress value owned by this thread.
 *      This function can only be called by a phase thread.
 * @param n The amount of progress steps to add.
 */
void increaseLocalProgress(ulong n) {
    localProgress->fetch_add(n);
};

/**
 * @brief Increases the max progress value of the associated phase.
 *      This function can only be called by a phase thread.
 * @param n The amount of progress steps to add.
 */
void increaseMaxProgress(ulong n) {
    maxProgress->increaseMax(n);
};

/**
 * @brief Decreases the max progress value of the associated phase.
 *      This function can only be called by a phase thread.
 * @param n The amount of progress steps to subtract.
 */
void decreaseMaxProgress(ulong n) {
    maxProgress->decreaseMax(n);
};


/**
 * @brief Increases the max progress value of the specified phase.
 * @param n The amount of progress steps to add.
 */
void increaseMaxProgress(PhaseID phaseId, ulong n) {
    phaseDataArrayLock.lock();
    phaseDataArray[phaseId].totalProgress->increaseMax(n);
    phaseDataArrayLock.unlock();
};

/**
 * @brief Decreases the max progress value of the specified phase.
 * @param n The amount of progress steps to subtract.
 */
void decreaseMaxProgress(PhaseID phaseId, ulong n) {
    phaseDataArrayLock.lock();
    phaseDataArray[phaseId].totalProgress->decreaseMax(n);
    phaseDataArrayLock.unlock();
};

/**
 * @brief Retrieves the max progress value of the specified phase.
 * @param n The max progress value.
 */
ulong fetchMaxProgress(PhaseID phaseId) {
    phaseDataArrayLock.lock();
    ulong r = phaseDataArray[phaseId].totalProgress->max.load();
    phaseDataArrayLock.unlock();
    return r;
};









/**
 * @brief initializes the data of each phase.
 *      This function MUST be called ONCE before starting any of the subphses
 */
void initPhaseData(){
    for(ulong i = 0; i < PhaseID::NUM; ++i) {
        phaseDataArrayLock.lock();
        phaseDataArray.push_back(PhaseData());
        phaseDataArrayLock.unlock();
    }
}






/**
 * @brief Checks if other threads have generated errors and stops the program if that's the case.
 *      This function can ONLY be called from the MAIN thread.
 *      Calling it from any other thread will break the compiler.
 */
void mainCheckErrors(){
    if(threadType != ThreadType::MAIN) {
        cerr++;
        cerr << "\nFatal: Error check function was called by a secondary thread. This is a bug and Lumen's developer is to blame for it.";
        cerr << "\nThe program was not stopped.";
        cerr--;
    }
    else {
        int exitCode = exitMainRequest.load();
        if(exitCode) {
            std::exit(exitCode);
        }
    }
}