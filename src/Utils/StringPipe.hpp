#pragma once
#include <thread>
#include <atomic>
#include <string>





 //*      Once the server thead has finished building the string, it MUST call .closePipe() to notify the other threads.




/**
 * @brief An std::string that allows client threads to access its characters while the server thread is still building the string.
 *      This string only contains a subset of the methods a standard std::string has.
 *      It cannot be resized and characters can only be appended using the += operator.
 *      All operations are thread-safe.
 *
 *      Accessing one or more characters is done through the [] operator and the .substr() function.
 *      When called, these methods will wait until the server thread has finished writing the requested characters.
 *      Do NOT use += from client threads. Doing so can cause race conditions.
 *      Accessing characters from the server thread is allowed, as long as the requested characters have been written.
 *      Use .isOpen() to check if the pipe is still being written to.
 *      Retrieving a '\0' means that the pipe was closed before reaching that index.
 *
 *      .cpp_str() returns a pointer to the raw std::string value.
 *      This method doesn't wait for the server thread to finish.
 */
struct StringPipe { // could be the name of an instrument
private:
    std::string s;
    std::atomic<ulong> len = 0;
    std::mutex sReallocLock;
    std::atomic<bool> _isOpen = true;
    // void wait(ulong i);




public:
    StringPipe() {}

    void operator+=(std::string &_s);
    void operator+=(char _s);
    void closePipe();

    bool isOpen();
    ulong length();
    // bool exists(ulong i);

    char operator[](ulong i);
    std::string substr(ulong i, ulong len);
    std::string* cpp_str();
};