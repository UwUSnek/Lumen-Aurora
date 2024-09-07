#include <iostream>
#include <streambuf>
#include <string>
#include <mutex>

#include "ALC.hpp"











// A mutex that controls the console output.
// This allows multiple threads to write multiple messages at once without being interrupted.
std::mutex consoleLock;


// std::vector<std::thread> threads;
// std::mutex threadsLock;
std::atomic<ulong> activeThreads = 0;
std::atomic<ulong> totalThreads = 0;
std::atomic<ulong> totalFiles = 0;
std::atomic<ulong> totalModules = 0;

// std::map<std::string, std::string*> fileContentCache;
thread_local ThreadType threadType = ThreadType::UNKNOWN;





// // Phase clocks and durations (they are set right before starting each phase and the duration calculated after it ends)
// std::chrono::_V2::system_clock::time_point timeStartPre;   std::chrono::duration<double> timePre;
// std::chrono::_V2::system_clock::time_point timeStartComp;  std::chrono::duration<double> timeComp;
// std::chrono::_V2::system_clock::time_point timeStartOpt;   std::chrono::duration<double> timeOpt;
// std::chrono::_V2::system_clock::time_point timeStartConv;  std::chrono::duration<double> timeConv;




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
        "Preprocessing",
        "Compilation",
        "Optimization",
        "Conversion",
    };
    return names[phaseId];
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
    maxProgress->increaseTot(n);
};