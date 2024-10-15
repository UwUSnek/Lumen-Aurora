#pragma once

#include <thread>
#include <mutex>
#include <vector>

#include "Utils/utils.hpp"
#include "Utils/DynamicProgressBar.hpp"


#define PRINT_DEBUG_INFO 1

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


    VersionNumber(){}

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
extern VersionNumber versionNumer;




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







extern std::mutex __internal_consoleLock;




class __internal_cout_stream_t : public std::streambuf {
protected:
    virtual int overflow(int c) override;
};
extern __internal_cout_stream_t __internal_cout_streambuff;
extern std::ostream __internal_cout;


class __internal_cout_stream_t_wrapper {
public:
    template<typename T> __internal_cout_stream_t_wrapper &operator<<(const T& val) {
        __internal_cout << val;
        return *this;
    }
    __internal_cout_stream_t_wrapper &operator++(int dummy) {
        __internal_consoleLock.lock();
        return *this;
    }

    __internal_cout_stream_t_wrapper &operator--(int dummy) {
        __internal_consoleLock.unlock();
        return *this;
    }
};
extern  __internal_cout_stream_t_wrapper cout;




class __internal_cerr_stream_t : public std::streambuf {
protected:
    virtual int overflow(int c) override;
};
extern __internal_cerr_stream_t __internal_cerr_streambuff;
extern std::ostream __internal_cerr;


class __internal_cerr_stream_t_wrapper {
public:
    template<typename T> __internal_cerr_stream_t_wrapper &operator<<(const T& val) {
        __internal_cerr << val;
        return *this;
    }
    __internal_cerr_stream_t_wrapper &operator++(int dummy) {
        __internal_consoleLock.lock();
        return *this;
    }

    __internal_cerr_stream_t_wrapper &operator--(int dummy) {
        __internal_consoleLock.unlock();
        return *this;
    }
};
extern  __internal_cerr_stream_t_wrapper cerr;









//TODO make this more readable, maybe group stuff in namespaces

#define LIST_PHASE_ID   \
    X(Preprocessing)    \
    X(Compilation)      \
    X(Optimization)     \
    X(Conversion)       \
    X(NUM)              \
    // ^ The number of phases, not including this enum value.


// Identifies the phases of the compilation progress
// Phases must be declared in order. Update phaseIdToString after changing this enum.
enum PhaseID : ulong {
    #define X(e) e,
    LIST_PHASE_ID
    #undef X
};
std::string phaseIdTotring(PhaseID phaseId);


struct PhaseData {
    DynamicProgressBar *totalProgress;
    // std::atomic<ulong> *activeSubphases;
    std::atomic<long>  *timeStart;
    std::atomic<long>  *timeEnd;

    PhaseData();
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
void decreaseMaxProgress(ulong n);

void increaseMaxProgress(PhaseID phaseId, ulong n);
void decreaseMaxProgress(PhaseID phaseId, ulong n);
ulong fetchMaxProgress(PhaseID phaseId);







//FIXME move to utils or something
#define MAX_THR_NAME_LEN 15


template<class func_t, class... args_t> void __internal_subphase_exec(PhaseID phaseId, bool isLast, std::atomic<bool> *initFeedback, func_t &&f, args_t &&...args) {

    // Set thread name and type
    threadType = ThreadType::SUBPHASE;
    std::string truncatedName = phaseIdTotring(phaseId).substr(0, MAX_THR_NAME_LEN - (1 /*Prefix "S"*/) - (1 /*Phase number*/) - (3 /*Separator*/));
    pthread_setname_np(pthread_self(), (std::string("S") + std::to_string(phaseId) + " | " + truncatedName).c_str());


    // Init thread counters
    activeThreads.fetch_add(1);
    totalThreads.fetch_add(1);


    // Set max progress pointer
    phaseDataArrayLock.lock();
    maxProgress = phaseDataArray[phaseId].totalProgress;
    phaseDataArrayLock.unlock();


    // Init phase starting time if needed
    phaseDataArrayLock.lock();
    if(*phaseDataArray[phaseId].timeStart == 0) {
        phaseDataArray[phaseId].timeStart->store(utils::getEpochMs());
    }
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

void initPhaseData();



//TODO move all newline prints to the beginning of the strings.
//TODO this is only for consistency reasons

void mainCheckErrors();