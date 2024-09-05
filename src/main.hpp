#include <thread>
#include <vector>
#include <mutex>
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"


extern std::vector<std::thread> threads;
extern std::mutex threadsLock;

extern std::vector<pre::SegmentedCleanSource*> pipes;
extern std::mutex pipesLock;