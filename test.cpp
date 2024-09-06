#include <iostream>
#include <thread>
#include <utility> // for std::forward

// Intermediate function that prints "hi" and then calls the requested function
template <typename Func, typename... Args>
void intermediateFunction(Func&& func, Args&&... args) {
    // Print "hi" before calling the requested function
    std::cout << "hi" << std::endl;

    // Call the actual function with the forwarded arguments
    std::forward<Func>(func)(std::forward<Args>(args)...);
}

// Custom thread function that mimics std::thread's behavior
template <typename Func, typename... Args>
std::thread customThread(Func&& func, Args&&... args) {
    // Use a lambda to wrap the call to intermediateFunction
    return std::thread([f = std::forward<Func>(func), ...args = std::forward<Args>(args)]() mutable {
        intermediateFunction(std::move(f), std::move(args)...);
    });
}

// Example function to be run in a thread
void exampleFunction(int x) {
    std::cout << "Running example function with value: " << x << std::endl;
}

int main() {
    // Create a thread using the customThread function
    std::thread t = customThread(exampleFunction, 42);

    // Wait for the thread to finish
    t.join();

    return 0;
}
