#pragma once
#include <optional>
#include <type_traits>

#include "__base_Pipe.hpp"









template <class t, class elmt> struct __base_BufferPipe : public __base_Pipe<t> {
private:
    std::atomic<ulong> len = 0;


protected:
    virtual ulong __internal_get_len(t    const &e) = 0;
    virtual void   __internal_append(t    const &e) = 0;
    virtual void   __internal_append(elmt const &e) = 0;








public:
    std::mutex sReallocLock;
    __base_BufferPipe() {}





    /**
     * @brief Adds every element of <_s> to the end of the pipe and makes them available to client threads, preserving their order.
     * @param _s The container to append.
     */
    void operator+=(t const &_s) {
        sReallocLock.lock();
        __internal_append(_s);
        len.fetch_add(__internal_get_len(_s));
        sReallocLock.unlock();
    }




    /**
     * @brief Adds the value <c> to the end of the pipe and makes it available to client threads.
     * @param c The value to add.
     */
    void operator+=(elmt const &c) {
        sReallocLock.lock();
        __internal_append(c);
        len.fetch_add(1);
        sReallocLock.unlock();
    }







    /**
     * @brief Returns the current length of the pipe.
     *      NOTICE: This might not be its final length. To know if the pipe has been fully written, use !.isOpen()
     * @return The length of the pipe.
     */
    ulong length() const {
        return len.load();
    }




    /**
     * @brief Retrieves the element at the requested index and returns a copy of it.
     * @param i The index.
     * @return A copy of the requested element wrapped in an std::optional, or an empty optional if the pipe was closed before reaching it.
     */
    std::optional<elmt> operator[](ulong i) {
        while(len.load() <= i) {
            if(!__base_Pipe<t>::isOpen() && len.load() <= i) return std::nullopt;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        sReallocLock.lock();
        elmt r = __base_Pipe<t>::s[i];
        sReallocLock.unlock();
        return r;
    }
};