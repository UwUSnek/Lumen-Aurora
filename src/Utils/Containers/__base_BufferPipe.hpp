#pragma once
#include <optional>
#include <type_traits>
#include <mutex>

#include "__base_Pipe.hpp"
#include "Main/ALC.hpp"








/**
 * @brief The base class of buffer pipes.
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

template <class t, class elmt, bool safeRealloc=true> struct __base_BufferPipe : public __base_Pipe<t> {
private:
    std::atomic<ulong> len = 0;
    std::mutex sReallocLock;


protected:
    virtual ulong __internal_get_len(t    const &e) const = 0;
    virtual void   __internal_append(t    const &e) = 0;
    virtual void   __internal_append(elmt const &e) = 0;


    /**
     * @brief A struct that acts as a scoped lock for __base_BufferPipe's reallocation operations.
     * Instances of this struct will keep the mutex locked while alive and release it once destroyed.
     */
    struct scoped_lock_t {
    private:
        std::unique_lock<std::mutex> lock_;
    public:
        explicit scoped_lock_t(__base_BufferPipe& parent)
            : lock_(parent.sReallocLock) {
            debug(if(safeRealloc == false) {
                throw std::logic_error("Attempted to create a scoped lock from a pipe with safeRealloc=false. This should never happen");
            })
        }
    };




public:

    /**
     * @brief Creates a scoped lock that stops other threads from reallocating this pipe's buffer.
     * This should be used each time the buffer is accessed without the thread-safe methods provided by the pipe.
     *
     * NOTICE: Calling a thread-safe method of the pipe while this lock is active will cause a deadlock. Don't do that.
     * @return The scoped lock. The lock is released once this goes out of scope.
     */
    scoped_lock_t scoped_lock() {
        return scoped_lock_t(*this);
    }


    __base_BufferPipe() = default;

    template<class ...u>
    explicit __base_BufferPipe(u &&...args)
        requires(!(std::same_as<std::remove_cvref_t<u>, __base_BufferPipe> || ...)) :
        __base_Pipe<t>(std::forward<u>(args)...) {
    }





    /**
     * @brief Adds every element of <_s> to the end of the pipe and makes them available to client threads, preserving their order.
     * @param _s The container to append.
     */
    void operator+=(t const &_s) {
        __internal_append(_s);
        len.fetch_add(__internal_get_len(_s), std::memory_order_release);
    }




    /**
     * @brief Adds the value <c> to the end of the pipe and makes it available to client threads.
     * @param c The value to add.
     */
    void operator+=(elmt const &c) {
        __internal_append(c);
        len.fetch_add(1, std::memory_order_release);
    }







    /**
     * @brief Returns the current length of the pipe.
     *      NOTICE: This might not be its final length. To know if the pipe has been fully written, use !.isOpen()
     * @return The length of the pipe.
     */
    ulong length() const {
        return len.load(std::memory_order_acquire);
    }




    /**
     * @brief Retrieves the element at the requested index and returns a copy of it.
     *     This function makes the thread wait until the element is available or the pipe is closed.
     * @param i The index.
     * @return A copy of the requested element wrapped in an optional, or nullopt if the pipe was closed before reaching the required size.
     */
    std::optional<elmt> operator[](ulong i) {


        // Wait for the element (or the pipe getting closed)
        while(len.load(std::memory_order_acquire) <= i) {
            if(!this->isOpen() && len.load(std::memory_order_acquire) <= i)  {
                return std::nullopt;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            //! A condition varable could be used instead of polling,
            //! but that can create unnecessary overhead as its logic can get quite complicated when multiple reader threads are involved.
            //! Polling every 10ms has a negligible overhead and is fast enough to not be noticeable by the user.
            //! 5-50ms is the weet spot.
            //! Waiting less than 5ms could work but is probably too fast. More than 50ms works too, but it adds noticeable delay between phases.
        }


        // Retrieve the element
        if constexpr(safeRealloc) {
            auto lock = scoped_lock();
            if(i >= (*this->cpp()).size()) return std::nullopt;
            return (*this->cpp())[i];
        }
        else {
            return (*this->cpp())[i];
        }
    }
};