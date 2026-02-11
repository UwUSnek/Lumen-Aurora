#pragma once
#include "__base_BufferPipe.hpp"
#include <cstring>




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
    ulong __internal_get_len(const std::string &e) override { auto lock = scoped_lock(); return e.length(); }
    void   __internal_append(const std::string &e) override { auto lock = scoped_lock(); cpp()->append(e); }
    void   __internal_append(const char &e)        override { auto lock = scoped_lock(); cpp()->push_back(e); }


public:
    StringPipe() = default;

    explicit StringPipe(ulong capacity) {
        cpp()->reserve(capacity);
    }

    explicit StringPipe(ulong size, char _char) :
        __base_BufferPipe<std::string, char>(size, _char) {
    }


    std::string substr(ulong i, ulong n) {
        auto lock = scoped_lock();
        return cpp()->substr(i, n);
    }


    bool strcmp(ulong i, const char* str) {
        auto lock = scoped_lock();
        return std::strcmp(cpp()->c_str() + i, str);
    }


    bool strcmp(ulong i, const std::string &str) {
        //! Scoped lock is created by the override
        return strcmp(i, str.c_str()); //NOSONAR
    }

    virtual ~StringPipe() = default;
};