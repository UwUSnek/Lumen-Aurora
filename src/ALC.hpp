#pragma once

#include <thread>

#include "Utils/utils.hpp"
#include "Utils/DynamicProgressBar.hpp"







extern std::atomic<int> exitMainRequest;
void exitMain(int exitCode);



extern std::vector<std::string> sourceFilePaths;
extern std::mutex sourceFilePathsLock;




extern std::mutex consoleLock;


extern std::atomic<ulong> activeThreads;
extern std::atomic<ulong> totalThreads;
extern std::atomic<ulong> totalFiles;
extern std::atomic<ulong> totalModules;


enum class ThreadType : ulong {
    MAIN        = 0,
    SUBPHASE    = 0xC0FFEE,
    UNKNOWN     = 0xDEADBEEF,
};
extern thread_local ThreadType threadType;


extern std::atomic<bool> isComplete;








class __internal_cout_stream_t : public std::streambuf {
private:
    std::mutex output_mutex;

protected:
    virtual int overflow(int c) override;
};
extern __internal_cout_stream_t __internal_cout_streambuff;
extern std::ostream cout;





class __internal_cerr_stream_t : public std::streambuf {
private:
    std::mutex output_mutex;

protected:
    virtual int overflow(int c) override;
};
extern __internal_cerr_stream_t __internal_cerr_streambuff;
extern std::ostream cerr;






//TODO make this more readable, maybe group stuff in namespaces

// Identifies the phases of the compilation progress
// Phases must be declared in order. Update phaseIdToString after changing this enum.
enum PhaseID : ulong {
    PREPROCESSING = 0,
    COMPILATION,
    OPTIMIZATION,
    CONVERSION,
    num // The number of phases, not including this enum value.
    //TODO add other phases
};
std::string phaseIdTotring(PhaseID phaseId);


struct PhaseData {
    DynamicProgressBar *totalProgress;
    // std::atomic<ulong> *activeSubphases;
    std::atomic<long>  *timeStart;
    std::atomic<long>  *timeEnd;

    PhaseData(long _timeStart);
};

// Data read by the display thread
extern std::vector<PhaseData> phaseDataArray;
extern std::mutex             phaseDataArrayLock;






struct SubphaseData {
    PhaseID phaseId;

    //! pointers avoid having to lock and unlock every time an element is accessed by a subphase thread
    std::atomic<ulong>*                         localProgress;

    SubphaseData(PhaseID _phaseId, std::atomic<ulong>* _localProgress) :
        phaseId(_phaseId),
        localProgress(_localProgress) {
    }
};

// Data read by the display thread
extern std::vector<SubphaseData> subphaseDataArray;
extern std::mutex                subphaseDataArrayLock;

// Per-thread data
extern thread_local std::atomic<ulong>* localProgress;
void increaseLocalProgress(ulong n);
extern thread_local DynamicProgressBar* maxProgress;
void increaseMaxProgress(ulong n);











template<class func_t, class... args_t> void __internal_subphase_exec(PhaseID phaseId, bool isLast, std::atomic<bool> *initFeedback, func_t &&f, args_t &&...args) {

    // Init thread data and counters
    threadType = ThreadType::SUBPHASE;
    activeThreads.fetch_add(1);
    totalThreads.fetch_add(1);


    // If needed, create new phase data (ordered), then increase its active subphases count
    phaseDataArrayLock.lock();
    if(phaseDataArray.size() <= phaseId) {
        phaseDataArray.push_back(PhaseData(utils::getEpochMs()));
    }
    maxProgress = phaseDataArray[phaseId].totalProgress;
    // phaseDataArray[phaseId].activeSubphases->fetch_add(1);
    phaseDataArrayLock.unlock();


    // Init subphase data (not ordered)
    localProgress = new std::atomic<ulong>(0);
    subphaseDataArrayLock.lock();
    subphaseDataArray.push_back(SubphaseData(phaseId, localProgress));
    subphaseDataArrayLock.unlock();


    // Start the actual function
    {
        initFeedback->store(true);
        std::forward<func_t>(f)(std::forward<args_t>(args)...);
    }


    // Set the ending time if needed
    phaseDataArrayLock.lock();
    if(isLast) phaseDataArray[phaseId].timeEnd->store(utils::getEpochMs());
    phaseDataArrayLock.unlock();


    // Update thread counter
    activeThreads.fetch_sub(1);
}




/**
 * @brief Starts a subphase thread using the specified task function and arguments.
 * @param phaseId The ID of the main phase.
 * @param isLast Whether this is the last subphase of the main phase.
 * @param f The task function to execute.
 * @param args The list of arguments to pass to <f>.
 *      All arguments are passed to the new thread by value.
 */
template<class func_t, class... args_t> void startSubphaseAsync(PhaseID phaseId, bool isLast, func_t &&f, args_t &&...args) {

    //! Subphase and Phase data initialization feedback
    std::atomic<bool> *isThreadDataInitialized = new std::atomic<bool>(false);


    // Start the new thread
    std::thread(
        [
            _phaseId = phaseId,
            _isThreadDataInitialized = isThreadDataInitialized,
            _isLast = isLast,
            _f = std::forward<func_t>(f),
            ..._args = std::forward<args_t>(args)
        ]() mutable {
            __internal_subphase_exec(_phaseId, _isLast, _isThreadDataInitialized, std::move(_f), std::move(_args)...);
        }
    ).detach();


    // Wait for the feedback before letting the main thread go
    //! Not sure if this is actually necessary, but im leaving it just in case it is.
    //! It's very difficult to debug and the overhead is negligible.
    while(!isThreadDataInitialized->load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    delete isThreadDataInitialized;
}