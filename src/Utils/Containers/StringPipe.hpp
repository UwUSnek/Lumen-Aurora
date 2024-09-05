#pragma once
#include <thread>
#include <atomic>
#include <string>

#include "BasePipe.hpp"









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
 *      Once the server thead has finished building the string, it MUST call .closePipe() to notify the other threads.
 *
 *      .cpp_str() returns a pointer to the raw std::string value.
 *      This method doesn't wait for the server thread to finish.
 */
struct StringPipe : public BasePipe<std::string, char> { // could be the name of an instrument
protected:
    ulong __internal_get_len(const std::string &e) override { return e.length(); }
    void   __internal_append(const std::string &e) override { s.append(e); }
    void   __internal_append(const char &e)        override { s.push_back(e); }


public:
    StringPipe() {}
    std::optional<std::string> substr(ulong i, ulong len);
};