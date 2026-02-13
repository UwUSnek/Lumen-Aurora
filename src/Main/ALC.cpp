#include <atomic>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <vector>

#include "Main/ALC.hpp"
#include "Utils/ansi.hpp"
#include "Main/FatalErrorException.hpp"
#include "Utils/console.hpp"




//TODO safely make threads terminate instead of killing the process when an error occurs
//TODO safely make threads terminate instead of killing the process when an error occurs




//! Set by the main function
VersionNumber *versionNumer = nullptr;




// The exit value specified by the thread that requested an exit. //! 0 means no threads exited yet.
std::atomic<int> exitMainRequest(0);


/**
 * @brief Signals the main thread there is an issue by throwing a FatalErrorException
 *     (which one of the parent functions is expected to catch). This makes it available for joining.
 *
 *     This function does NOT prevent other threads from printing to the console while it is being executed.
 *     External synchronization is required.
 *
 *     If the current thread is the main thread, this is equivalent to calling exit()
 */
void exitMain(int exitCode) { //NOSONAR
    if(threadType == ThreadType::MAIN) {
        std::exit(exitCode);
    }
    else {
        exitMainRequest.store(exitCode);
        // while(!isComplete.load(std::memory_order_acquire)) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        throw(FatalErrorException(exitCode));
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








// Becomes true when the main thread is ready to return.
std::atomic<bool> isComplete(false);











std::string phaseIdTotring(PhaseID phaseId) {
    static const std::vector<std::string> names = {
        #define X(e) std::string(#e),
        LIST_PHASE_ID
        #undef X
    };
    return names[(ulong)phaseId];
}



PhaseData::PhaseData() :
    totalProgress(newptr<DynamicProgressBar>(0)),
    timeStart    (newptr<std::atomic<long>>(0)),
    timeEnd      (newptr<std::atomic<long>>(0)) {
}


std::vector<PhaseData> phaseDataArray;
std::mutex             phaseDataArrayLock;

std::vector<SubphaseData> subphaseDataArray;
std::mutex                subphaseDataArrayLock;

thread_local ptr<std::atomic<ulong>> localProgress = nullptr;
thread_local ptr<DynamicProgressBar>   maxProgress = nullptr;


/**
 * @brief Increases the local progress value owned by this thread.
 *      This function can only be called by a phase thread.
 * @param n The amount of progress steps to add.
 */
void increaseLocalProgress(ulong n) {
    localProgress->fetch_add(n);
};

/**
 * @brief Flags the phase as having encountered an issue.
 *      This function can only be called by a phase thread.
 */
void flagLocalError() {
    maxProgress->flagError();
}

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
 * @brief Retrieves the max progress value of the specified phase.
 * @param n The max progress value.
 */
ulong fetchMaxProgress(PhaseID phaseId) {
    std::scoped_lock lock(phaseDataArrayLock);
    ulong r = phaseDataArray[(ulong)phaseId].totalProgress->getMax();
    return r;
};








/**
 * @brief initializes the data of each phase.
 *      This function MUST be called ONCE before starting any of the subphses
 */
void initPhaseData(){
    std::scoped_lock lock(phaseDataArrayLock);
    for(ulong i = 0; i < (ulong)PhaseID::NUM; ++i) {
        phaseDataArray.emplace_back();
    }
}




/**
 * @brief Checks if other threads have generated errors.
 *      This function can ONLY be called from the MAIN thread.
 *      Calling it from any other thread will break the compiler.
 * @return False if other threads have generated an error, true otherwise.
 */
bool mainCheckErrors(){
    if(threadType != ThreadType::MAIN) {
        console::cerr++;
        console::cerr << "\nFatal: Error check function was called by a secondary thread. This is a bug and Lumen's developer is to blame for it.";
        console::cerr << "\nThe program was not stopped.";
        console::cerr--;
        return false;
    }
    else {
        return !exitMainRequest.load();
    }
}