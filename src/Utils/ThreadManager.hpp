
#include <thread>
#include <vector>




struct ThreadManager {
private:
    inline static std::vector<std::jthread> threads;

public:
    inline static void addThread(std::jthread &&thread) {
        threads.push_back(std::move(thread));
    }
};