#pragma once
#include "__base_Pipe.hpp"




/**
 * @brief An std::vector that allows client threads to access its elements while the server thread is still building the vector.
 *      It cannot be resized and elements can only be appended using the += operator.
 *      All operations are thread-safe.
 *
 *      Accessing elements is done through the [] operator.
 *      When called, this method will wait until the server thread has finished writing the requested elements.
 *      Retrieving an optional with no value means that the pipe has been closed before the character could be written.
 *
 *      Use .isOpen() to check if the pipe is still being written to.
 *      Once the server thead has finished building the string, it MUST call .closePipe() to notify the other threads.
 */
template<class t> struct VectorPipe : public __base_Pipe<std::vector<t>, t> {
protected:
    ulong __internal_get_len(const std::vector<t> &e) override { return e.size(); }
    void   __internal_append(const std::vector<t> &e) override { __base_Pipe<std::vector<t>, t>::s.insert(__base_Pipe<std::vector<t>, t>::s.end(), e.begin(), e.end()); }
    void   __internal_append(const t &e)              override { __base_Pipe<std::vector<t>, t>::s.push_back(e); }
};
