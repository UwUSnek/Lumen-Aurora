#include <thread>
#include <vector>
#include <mutex>
#include <map>
#include <atomic>

#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"
#include "Utils/DynamicProgressBar.hpp"






extern std::mutex consoleLock;


extern std::vector<std::thread> threads;
extern std::mutex threadsLock;
extern std::atomic<ulong> activeThreads;

// extern std::map<std::string, std::string*> fileContentCache;




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
