#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <map>
#include <atomic>
#include <functional>

#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"
#include "Utils/DynamicProgressBar.hpp"





extern std::vector<std::string> sourceFilePaths;
extern std::mutex sourceFilePathsLock;




extern std::mutex consoleLock;


// extern std::vector<std::thread> threads;
// extern std::mutex threadsLock;
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


//TODO replace with a more automated system

// extern std::chrono::_V2::system_clock::time_point timeStartPre;   extern std::chrono::duration<double> timePre;
// extern std::chrono::_V2::system_clock::time_point timeStartComp;  extern std::chrono::duration<double> timeComp;
// extern std::chrono::_V2::system_clock::time_point timeStartOpt;   extern std::chrono::duration<double> timeOpt;
// extern std::chrono::_V2::system_clock::time_point timeStartConv;  extern std::chrono::duration<double> timeConv;

// extern std::map<std::string, std::string*> fileContentCache;


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
    std::atomic<ulong> *activeSubphases;
    std::atomic<long>  *timeStart;
    std::atomic<long>  *timeEnd;

    PhaseData(long _timeStart) :
        totalProgress(new DynamicProgressBar(0, ansi::bright_green, ansi::bright_black)),
        activeSubphases(new std::atomic<ulong>(0)),
        timeStart      (new std::atomic<long>(_timeStart)),
        timeEnd        (new std::atomic<long>(0)) {
    }
};

// Data read by the display thread
extern std::vector<PhaseData> phaseDataArray;
extern std::mutex             phaseDataArrayLock;






struct SubphaseData {
    PhaseID phaseId;

    //TODO add something that points to the main phase this subphase is part of
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










template<class func_t, class... args_t> void __internal_subphase_exec(PhaseID phaseId, func_t &&f, args_t &&...args) {

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
    phaseDataArray[phaseId].activeSubphases->fetch_add(1);
    phaseDataArrayLock.unlock();


    // Init subphase data (not ordered)
    localProgress = new std::atomic<ulong>(0);
    subphaseDataArrayLock.lock();
    subphaseDataArray.push_back(SubphaseData(phaseId, localProgress));
    subphaseDataArrayLock.unlock();


    // Start the actual function
    std::forward<func_t>(f)(std::forward<args_t>(args)...);


    // Decrease active subphases count and set the end time if needed
    phaseDataArrayLock.lock();
    phaseDataArray[phaseId].activeSubphases->fetch_sub(1);
    if(phaseDataArray[phaseId].activeSubphases->load() == 0) {
        phaseDataArray[phaseId].timeEnd->store(utils::getEpochMs());
    }
    phaseDataArrayLock.unlock();


    // Update thread counter
    activeThreads.fetch_sub(1);
}




template<class func_t, class... args_t> void startSubphaseAsync(PhaseID phaseId, func_t &&f, args_t &&...args) {
    std::thread(
        [lambda_phaseId = phaseId, lambda_f = std::forward<func_t>(f), ...lambda_args = std::forward<args_t>(args)]() mutable {
            __internal_subphase_exec(lambda_phaseId, std::move(lambda_f), std::move(lambda_args)...);
        }
    ).detach();
}


// struct PhaseData {

//     // The name of the phase
//     std::string name;

//     // The progress bar that shows up in the output
//     // This is only accessed by the server thread
//     DynamicProgressBar totalProgress;

//     // Local progress data
//     //! Worker threads MUST use a thread_local pointer to their element in order to access it
//     // Modifying the vector itself requires using localProgressArrayLock. Any thread can modify it
//     std::vector<std::atomic<ulong>*> localProgressArray;
//     std::mutex                       localProgressArrayLock;


//     PhaseData() : totalProgress(0, ansi::bright_green, ansi::bright_black) {}
// };




// struct SubphaseData {
//     std::string name;
//     pre::SegmentedCleanSource outputPipe;
// };
// extern std::vector<SubphaseData*> subphases;
// extern std::mutex subphasesLock;

// void
