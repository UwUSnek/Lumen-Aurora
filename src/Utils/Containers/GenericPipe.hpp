#pragma once
#include <mutex>
#include "__base_Pipe.hpp"




/**
 * @brief A pipe wrapper for types that cannot be treated as arrays.
 *      This struct provides a mutex to safely perform operations on the wrapped value
 *      and the awaitClose() and closePipe() methods from __base_Pipe.
 *      Concurrent read/write operations are allowed, thought there is no specific method for them.
 */
template<class t> struct GenericPipe : public __base_Pipe<t> {
private:
    std::mutex m;

public:
    void   lock(){ m.  lock(); }
    void unlock(){ m.unlock(); }
};