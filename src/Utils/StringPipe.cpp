#include "StringPipe.hpp"




// /**
//  * @brief Makes the thread wait until the character at index <i> is written by the server thread.
//  * @param i The index of the character to wait for.
//  */
// void StringPipe::wait(ulong i) {
//     while(len.load() <= i) {
//         std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     }
// }





void StringPipe::operator+=(std::string &_s) {
    if(s.length() + _s.length() >= s.capacity()) {
        sReallocLock.lock();
        s += _s;
        len.fetch_add(_s.length());
        sReallocLock.unlock();
    }
    else {
        s += _s;
        len.fetch_add(_s.length());
    }
}
void StringPipe::operator+=(char c) {
    if(s.length() + 1 >= s.capacity()) {
        sReallocLock.lock();
        s += c;
        len.fetch_add(1);
        sReallocLock.unlock();
    }
    else {
        s += c;
        len.fetch_add(1);
    }
}



void StringPipe::closePipe() {
    _isOpen.store(false);
}

bool StringPipe::isOpen() {
    return _isOpen;
}

/**
 * @brief Returns the current length of the string.
 *      NOTICE: This might not be its final length. To know if the string has been fully written, use !.isOpen()
 * @return The length of the string.
 */
ulong StringPipe::length() {
    sReallocLock.lock();
    auto r = s.length();
    sReallocLock.unlock();
    return r;
}

// /**
//  * @brief Check if a certain index exists.
//  *      If not, tries to wait for it. Returns false if the pipe gets closed before reaching it.
//  * @param i The index to check.
//  * @return true if the index exists, false otherwise.
//  */
// bool exists(ulong i) {

// }




/**
 * @brief //TODO
 * @param i //TODO
 * @return The requested character, or '\0' if the pipe was closed before reaching it.
 */
char StringPipe::operator[](ulong i) {
    // wait(i);
    while(len.load() <= i) {
        if(!isOpen() && len.load() <= i) return '\0';
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    sReallocLock.lock();
    char r = s[i];
    sReallocLock.unlock();
    return r;
}




/**
 * @brief //TODO
 * @param i //TODO
 * @return The requested substring, or an empty std::string if the pipe was closed before reaching the last requested character.
 */
std::string StringPipe::substr(ulong i, ulong n) {
    // wait(i + len);
    while(len.load() <= i + n) {
        if(!isOpen() && len.load() <= i + n) return "";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    sReallocLock.lock();
    std::string&& r = s.substr(i, n);
    sReallocLock.unlock();
    return r;
}




/**
 * @brief Returns the address to the std::string value used by this StringPipe.
 *      Operations on this object are only thread-safe if .isOpen() returns false.
 * @return A pointer to the string object.
 */
std::string* StringPipe::cpp_str() {
    return &s;
}



