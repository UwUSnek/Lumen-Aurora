#include <iostream>

#include "ALC.hpp"
#include "Utils/ansi.hpp"










// The exit value specified by the thread that requested an exit. //! 0 means no threads exited yet.
std::atomic<int> exitMainRequest = 0;


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



// A mutex that controls the console output.
// This allows multiple threads to write multiple messages at once without being interrupted.
std::mutex consoleLock;


std::atomic<ulong> activeThreads = 0;
std::atomic<ulong> totalThreads = 0;
std::atomic<ulong> totalFiles = 0;
std::atomic<ulong> totalModules = 0;

thread_local ThreadType threadType = ThreadType::UNKNOWN;








//FIXME detect all the files before processing them to make progress more reliable
// Becomes true after the output file gets written and closed
std::atomic<bool> isComplete(false);










int __internal_cout_stream_t::overflow(int c) {
    if (c != EOF) {
        output_mutex.lock();
        if (c == '\n') std::cout << "\033[K";
        std::cout.put(c);
        output_mutex.unlock();
    }
    return c;
}
__internal_cout_stream_t __internal_cout_streambuff;
std::ostream cout(&__internal_cout_streambuff);





int __internal_cerr_stream_t::overflow(int c) {
    if (c != EOF) {
        output_mutex.lock();
        if (c == '\n') std::cerr << "\033[K";
        std::cerr.put(c);
        output_mutex.unlock();
    }
    return c;
}
__internal_cerr_stream_t __internal_cerr_streambuff;
std::ostream cerr(&__internal_cerr_streambuff);









std::string phaseIdTotring(PhaseID phaseId) {
    static const char* names[] = {
        #define X(e) #e,
        LIST_PHASE_ID
        #undef X
    };
    return names[phaseId];
}



PhaseData::PhaseData(long _timeStart) :
    totalProgress(new DynamicProgressBar(0, ansi::bright_green, ansi::bright_black)),
    timeStart      (new std::atomic<long>(_timeStart)),
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