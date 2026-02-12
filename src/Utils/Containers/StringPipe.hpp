#pragma once
#include "__base_BufferPipe.hpp"
#include <cstring>




/**
 * @brief An std::string that allows client threads to access its characters while the server thread is still building the string.
 *      It cannot be resized and characters can only be appended using the += operator.
 *      All operations are thread-safe, unless safeRealloc=false is used. In that case, only .length() and .isOpen() are thread-safe.
 *
 *      Accessing characters is done through the [] operator.
 *      When called, this method will wait until the server thread has finished writing the requested characters.
 *      Retrieving an optional with no value means that the pipe has been closed before the requested character could be written.
 *
 *      Use .isOpen() to check if the pipe is still being written to.
 *      Once the server thead has finished building the string, it MUST call .closePipe() to notify the other threads.
 */
template<bool safeRealloc=true> struct StringPipe : public __base_BufferPipe<std::string, char, safeRealloc> {
protected:
    ulong __internal_get_len(const std::string &e) const override {
        return e.length();
    }


    void __internal_append(const std::string &e) override {
        if constexpr(safeRealloc) {
            auto lock = this->scoped_lock();
            __internal_finalizeAppend(e);
        } else {
            __internal_finalizeAppend(e);
        }
    }
    void __internal_finalizeAppend(const std::string &e) {
        this->cpp()->append(e);
    }


    void __internal_append(const char &e) override {
        if constexpr(safeRealloc) {
            auto lock = this->scoped_lock();
            __internal_finalizeAppend(e);
        } else {
            __internal_finalizeAppend(e);
        }
    }
    void __internal_finalizeAppend(const char &e) {
        this->cpp()->push_back(e);
    }


public:
    StringPipe() = default;

    explicit StringPipe(ulong capacity) {
        this->cpp()->reserve(capacity);
    }

    explicit StringPipe(ulong size, char _char) :
        __base_BufferPipe<std::string, char>(size, _char) {
    }


    std::string substr(ulong i, ulong n) requires(safeRealloc == true) {
        auto lock = this->scoped_lock();
        return __internal_finalizeSubstr(i, n);
    }
    std::string substr(ulong i, ulong n) requires(safeRealloc == false) {
        return __internal_finalizeSubstr(i, n);
    }
    std::string __internal_finalizeSubstr(ulong i, ulong n) {
        return this->cpp()->substr(i, n);
    }


    bool strcmp(ulong i, const char* str) requires(safeRealloc == true) {
        auto lock = this->scoped_lock();
        return __internal_finalizeStrcmp(i, str);
    }
    bool strcmp(ulong i, const char* str) requires(safeRealloc == false) {
        return __internal_finalizeStrcmp(i, str);
    }
    bool __internal_finalizeStrcmp(ulong i, const char* str) {
        return std::strcmp(this->cpp()->c_str() + i, str);
    }


    bool strcmp(ulong i, const std::string &str) {
        //! Scoped lock is handled by the override
        return strcmp(i, str.c_str()); //NOSONAR
    }

    virtual ~StringPipe() = default;
};