#include "StringPipe.hpp"








/**
 * @brief Creates a new string containing the substring of this StringPipe that strarts at index <i> and spand <n> characters.
 * @param i The starting index of the substring.
 * @param n The length of the substring.
 * @return The requested substring wrapped in an std::optional, or an empty optional if the pipe was closed before reaching the last requested character.
 */
std::optional<std::string> StringPipe::substr(ulong i, ulong n) {
    // // wait(i + len);
    // while(len.load() <= i + n) {
    //     if(!isOpen() && len.load() <= i + n) return "";
    //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // }
    if(!operator[](i + n).has_value()) return std::nullopt; //TODO optimize

    sReallocLock.lock();
    std::string&& r = s.substr(i, n);
    sReallocLock.unlock();
    return r;
}

