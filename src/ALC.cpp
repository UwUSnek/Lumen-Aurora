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






// Phase clocks and durations (they are set right before starting each phase and the duration calculated after it ends)
std::chrono::_V2::system_clock::time_point timeStartPre;   std::chrono::duration<double> timePre;
std::chrono::_V2::system_clock::time_point timeStartComp;  std::chrono::duration<double> timeComp;
std::chrono::_V2::system_clock::time_point timeStartOpt;   std::chrono::duration<double> timeOpt;
std::chrono::_V2::system_clock::time_point timeStartConv;  std::chrono::duration<double> timeConv;




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