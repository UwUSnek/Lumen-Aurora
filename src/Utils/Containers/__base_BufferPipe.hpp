#pragma once
#include <optional>
#include <type_traits>
#include <mutex>

#include "__base_Pipe.hpp"









template <class t, class elmt> struct __base_BufferPipe : public __base_Pipe<t> {
private:
    std::atomic<ulong> len = 0;
    std::mutex sReallocLock;


protected:
    virtual ulong __internal_get_len(t    const &e) = 0;
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
        }
    };




public:

    /**
     * @brief Creates a scoped lock that stops other threads from reallocating this pipe's buffer.
     * This should be used each time the buffer is accessed without the thread-safe methods provided by the pipe.
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
        std::scoped_lock lock(sReallocLock);
        __internal_append(_s);
        len.fetch_add(__internal_get_len(_s), std::memory_order_release);
    }




    /**
     * @brief Adds the value <c> to the end of the pipe and makes it available to client threads.
     * @param c The value to add.
     */
    void operator+=(elmt const &c) {
        std::scoped_lock lock(sReallocLock);
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
     * @param i The index.
     * @return A copy of the requested element wrapped in an std::optional, or an empty optional if the pipe was closed before reaching the required size.
     */
    std::optional<elmt> operator[](ulong i) {
        while(len.load(std::memory_order_acquire) <= i) {
            if(!__base_Pipe<t>::isOpen() && len.load(std::memory_order_acquire) <= i)  {
                return std::nullopt;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::scoped_lock lock(sReallocLock);
        return __base_Pipe<t>::s[i];
    }
};