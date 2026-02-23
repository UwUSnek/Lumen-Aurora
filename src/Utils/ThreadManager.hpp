#pragma once
#include <thread>
#include <vector>




struct ThreadManager {
private:
    inline static std::vector<std::jthread> threads;


public:

    /**
     * Adds a new thread to the ThreadManager.
     * @param thread The thread to add.
     */
    inline static void addThread(std::jthread &&thread) {
        threads.push_back(std::move(thread));
    }

    /**
     * @brief Waits for all the threads to exit their function.
     * This blocks the caller thread.
     */
    inline static void joinAll() {
        for(auto &t : threads) {
            t.join();
        }
    }
};