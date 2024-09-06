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






extern std::mutex consoleLock;


// extern std::vector<std::thread> threads;
// extern std::mutex threadsLock;
extern std::atomic<ulong> activeThreads;
extern std::atomic<ulong> totalThreads;
extern std::atomic<ulong> totalFiles;
extern std::atomic<ulong> totalModules;



//TODO replace with a more automated system

extern std::chrono::_V2::system_clock::time_point timeStartPre;   extern std::chrono::duration<double> timePre;
extern std::chrono::_V2::system_clock::time_point timeStartComp;  extern std::chrono::duration<double> timeComp;
extern std::chrono::_V2::system_clock::time_point timeStartOpt;   extern std::chrono::duration<double> timeOpt;
extern std::chrono::_V2::system_clock::time_point timeStartConv;  extern std::chrono::duration<double> timeConv;

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







template<class func_t, class... args_t> void __internal_phase_exec(func_t &&f, args_t &&...args) {
    activeThreads.fetch_add(1);
    totalThreads.fetch_add(1);
    std::forward<func_t>(f)(std::forward<args_t>(args)...);
    activeThreads.fetch_sub(1);
}


template<class func_t, class... args_t> void startPhaseAsync(func_t &&f, args_t &&...args) {
    std::thread(
        [lambda = std::forward<func_t>(f), ...lambda_args = std::forward<args_t>(args)]() mutable {
            __internal_phase_exec(std::move(lambda), std::move(lambda_args)...);
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
