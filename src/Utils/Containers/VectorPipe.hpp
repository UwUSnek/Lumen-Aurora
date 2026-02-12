#pragma once
#include "__base_BufferPipe.hpp"




/**
 * @brief An std::vector that allows client threads to access its elements while the server thread is still building the vector.
 *      It cannot be resized and elements can only be appended using the += operator.
 *      All operations are thread-safe, unless safeRealloc=false is used. In that case, only .length() and .isOpen() are thread-safe.
 *
 *      Accessing elements is done through the [] operator.
 *      When called, this method will wait until the server thread has finished writing the requested elements.
 *      Retrieving an optional with no value means that the pipe has been closed before the requested element could be written.
 *
 *      Use .isOpen() to check if the pipe is still being written to.
 *      Once the server thead has finished building the buffer, it MUST call .closePipe() to notify the other threads.
 */
template<class t, bool safeRealloc> struct VectorPipe : public __base_BufferPipe<std::vector<t>, t> {
protected:
    ulong __internal_get_len(const std::vector<t> &e) const override {
        return e.size();
    }


    void __internal_append(const std::vector<t> &e) override {
        if constexpr(safeRealloc) {
            auto lock = this->scoped_lock();
            __internal_finalizeAppend(e);
        }
        else {
            __internal_finalizeAppend(e);
        }
    }
    void __internal_finalizeAppend(const std::vector<t> &e) {
        this->cpp()->insert(this->cpp()->end(), e.begin(), e.end());
    }


    void __internal_append(const t &e) override {
        if constexpr(safeRealloc) {
            auto lock = this->scoped_lock();
            __internal_finalizeAppend(e);
        }
        else {
            __internal_finalizeAppend(e);
        }
    }
    void __internal_finalizeAppend(const t &e) {
        this->cpp()->push_back(e);
    }


public:
    VectorPipe() = default;

    explicit VectorPipe(ulong capacity) :
        __base_BufferPipe<std::vector<t>, t>(capacity){
    }

    explicit VectorPipe(ulong size, t value) :
        __base_BufferPipe<std::vector<t>, t>(size, value){
    }

    virtual ~VectorPipe() = default;
};
