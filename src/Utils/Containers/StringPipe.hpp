#pragma once
#include "__base_BufferPipe.hpp"




/**
 * @brief An std::string that allows client threads to access its characters while the server thread is still building the string.
 *      It cannot be resized and characters can only be appended using the += operator.
 *      All operations are thread-safe.
 *
 *      Accessing characters is done through the [] operator.
 *      When called, this method will wait until the server thread has finished writing the requested characters.
 *      Retrieving an optional with no value means that the pipe has been closed before the character could be written.
 *
 *      Use .isOpen() to check if the pipe is still being written to.
 *      Once the server thead has finished building the string, it MUST call .closePipe() to notify the other threads.
 */
struct StringPipe : public __base_BufferPipe<std::string, char> {
protected:
    ulong __internal_get_len(const std::string &e) override { return e.length(); }
    void   __internal_append(const std::string &e) override { s.append(e); }
    void   __internal_append(const char &e)        override { s.push_back(e); }
};