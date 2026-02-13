#pragma once

#include <concepts>
#include <thread>
#include <mutex>
#include <type_traits>
#include <vector>

#include "Utils/utils.hpp"
#include "Utils/DynamicProgressBar.hpp"
#include "Utils/ThreadManager.hpp"
#include "Utils/ptr.hpp"


//FIXME actually control this from somewhere
#define PRINT_DEBUG_INFO 0

#if PRINT_DEBUG_INFO == 1
    #define debug(...) __VA_ARGS__
#else
    #define debug(...)
#endif





struct VersionNumber {
    char platform;
    uint major;
    uint minor;
    uint patch;
    char label;


    VersionNumber() = delete;

    /**
     * @brief Construct a new Version Number object (Specify a label)
     * @param _platform 'L' or 'W' for Linux or Windows respectively
     * @param _major The major version number
     * @param _minor The minor version number
     * @param _patch The number of the patch
     * @param _label The label (alpha, beta etc...)
     */
    VersionNumber(char _platform, uint _major, uint _minor, uint _patch, char _label) :
        platform(_platform),
        major(_major),
        minor(_minor),
        patch(_patch),
        label(_label) {
    }

    /**
     * @brief Construct a new Version Number object (No label)
     * @param _platform 'L' or 'W' for Linux or Windows respectively
     * @param _major The major version number
     * @param _minor The minor version number
     * @param _patch The number of the patch
     */
    VersionNumber(char _platform, uint _major, uint _minor, uint _patch) :
        VersionNumber(_platform, _major, _minor, _patch, '\0') {
    }
};
extern VersionNumber *versionNumer;




extern std::atomic<int> exitMainRequest;
void exitMain(int exitCode);



extern std::vector<std::string> sourceFilePaths;
extern std::mutex sourceFilePathsLock;






extern std::atomic<ulong> activeThreads;
extern std::atomic<ulong> totalThreads;
extern std::atomic<ulong> totalFiles;
extern std::atomic<ulong> totalModules;


enum class ThreadType : ulong {
    MAIN        = 0,
    SUBPHASE    = 0xC0FFEE,
    MONITOR     = 0xBEE,
    UNKNOWN     = 0xDEAD,
};
extern thread_local ThreadType threadType;


extern std::atomic<bool> isComplete;









//TODO make this more readable, maybe group stuff in namespaces

#define LIST_PHASE_ID       \
    X(P0_Includes)          \
    X(P1_LineSplicing)      \
    X(P2_Cleanup)           \
    X(P3_Macros)            \
    X(C0_Tokenization)      \
    X(C1_TreeCreation)      \
    X(C2_Optimization)      \
    X(C3_Conversion)        \
    X(NUM)                  \
    // ^ The number of phases, not including this enum value.


// Identifies the phases of the compilation progress
// Phases must be declared in order. Update phaseIdToString after changing this enum.
enum class PhaseID : ulong {
    #define X(e) e,
    LIST_PHASE_ID
    #undef X
};
std::string phaseIdTotring(PhaseID phaseId);


struct PhaseData {
    ptr<DynamicProgressBar> totalProgress;
    ptr<std::atomic<long>>  timeStart;
    ptr<std::atomic<long>>  timeEnd;

    PhaseData();
};

// Data read by the display thread
extern std::vector<PhaseData> phaseDataArray;
extern std::mutex             phaseDataArrayLock;






struct SubphaseData {
    PhaseID phaseId;

    //! pointers avoid having to lock and unlock every time an element is accessed by a subphase thread
    ptr<std::atomic<ulong>> localProgress;

    SubphaseData(PhaseID _phaseId, ptr<std::atomic<ulong>> _localProgress) :
        phaseId(_phaseId),
        localProgress(_localProgress) {
    }
};

// Data read by the display thread
extern std::vector<SubphaseData> subphaseDataArray;
extern std::mutex                subphaseDataArrayLock;

// Per-thread data
extern thread_local ptr<std::atomic<ulong>> localProgress;
extern thread_local ptr<DynamicProgressBar> maxProgress;
void increaseLocalProgress(ulong n);
void flagLocalError();
void increaseMaxProgress(ulong n);
void decreaseMaxProgress(ulong n);
ulong fetchMaxProgress(PhaseID phaseId);


/**
 * @brief Increases the max progress value of the specified phases.
 * @param n The amount of progress steps to add.
 * @param phaseIDs The IDs of the phases to affect.
 */
template<class ...t> void increaseMaxProgress(ulong n, t ...phaseIDs) requires(std::same_as<t, PhaseID> && ...) {
    std::scoped_lock lock(phaseDataArrayLock);
    ((phaseDataArray[(ulong)phaseIDs].totalProgress->increaseMax(n)), ...);
};

/**
 * @brief Decreases the max progress value of the specified phases.
 * @param n The amount of progress steps to subtract.
 * @param phaseIDs The IDs of the phases to affect.
 */
template<class ...t> void decreaseMaxProgress(ulong n, t ...phaseIDs) requires(std::same_as<t, PhaseID> && ...) {
    std::scoped_lock lock(phaseDataArrayLock);
    (phaseDataArray[(ulong)phaseIDs].totalProgress->decreaseMax(n), ...);
};





//FIXME move to utils or something
#define MAX_THR_NAME_LEN 15


template<class func_t, class... args_t> void __internal_subphase_exec(PhaseID phaseId, bool isLast, ptr<std::atomic<bool>> initFeedback, func_t &&f, args_t &&...args) {

    // Set thread name and type
    threadType = ThreadType::SUBPHASE;
    std::string truncatedName = phaseIdTotring(phaseId).substr(0, MAX_THR_NAME_LEN);
    char threadName[MAX_THR_NAME_LEN]; //NOSONAR
    snprintf(threadName, sizeof(threadName), "%s", truncatedName.c_str()); //NOSONAR
    pthread_setname_np(pthread_self(), threadName);


    // Init thread counters
    activeThreads.fetch_add(1);
    totalThreads.fetch_add(1);


    { // Set max progress pointer
        std::scoped_lock lock(phaseDataArrayLock);
        maxProgress = phaseDataArray[(ulong)phaseId].totalProgress;
    }


    { // Init phase starting time if needed
        std::scoped_lock lock(phaseDataArrayLock);
        if(*phaseDataArray[(ulong)phaseId].timeStart == 0) {
            phaseDataArray[(ulong)phaseId].timeStart->store(utils::getEpochMs());
        }
    }


    { // Init subphase data (not ordered)
        localProgress = newptr<std::atomic<ulong>>(0);
        std::scoped_lock lock(subphaseDataArrayLock);
        subphaseDataArray.emplace_back(phaseId, localProgress);
    }


    { // Start the actual function
        initFeedback->store(true);
        std::forward<func_t>(f)(std::forward<args_t>(args)...);
    }


    { // Set the ending time if needed
        std::scoped_lock lock(phaseDataArrayLock);
        if(isLast) phaseDataArray[(ulong)phaseId].timeEnd->store(utils::getEpochMs());
    }


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
template<class func_t, class... args_t> void startSubphaseAsync(PhaseID phaseId, bool isLast, const func_t &f, const args_t& ...args) {

    //! Subphase and Phase data initialization feedback
    auto isThreadDataInitialized = newptr<std::atomic<bool>>(false);


    // Start the new thread
    ThreadManager::addThread(std::jthread(
        [phaseId = phaseId, isLast = isLast, isThreadDataInitialized = isThreadDataInitialized, f = f, ...args = args](){
            __internal_subphase_exec(phaseId, isLast, isThreadDataInitialized, f, args...);
        }
    ));


    // Wait for the feedback before letting the main thread go
    //! Not sure if this is actually necessary, but im leaving it just in case it is.
    //! It's very difficult to debug and the overhead is negligible.
    while(!isThreadDataInitialized->load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void initPhaseData();



//TODO move all newline prints to the beginning of the strings.
//TODO this is only for consistency reasons

bool mainCheckErrors();