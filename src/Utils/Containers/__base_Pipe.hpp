#pragma once
#include <thread>
#include <atomic>
#include <optional>
#include <type_traits>









template <class t, class elmt> struct __base_Pipe { // could be the name of an instrument
private:
    std::atomic<ulong> len = 0;
    std::atomic<bool> _isOpen = true;


protected:
    t s;
    virtual ulong __internal_get_len(t    const &e) = 0;
    virtual void   __internal_append(t    const &e) = 0;
    virtual void   __internal_append(elmt const &e) = 0;








public:
    std::mutex sReallocLock;


    __base_Pipe() {}





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
     * @brief Closes the pipe.
     *      This function must be exclusively called from the server thread.
     */
    void closePipe() {
        _isOpen.store(false);
    }


    /**
     * @brief Checks if the pipe is still open
     * @return true if the pipe is open, false otherwise
     */
    inline bool isOpen() const {
        return _isOpen.load();
    }


    /**
     * @brief Makes the thread sleep until the pipe closes.
     */
    void awaitClose() const {
        while(isOpen()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
            if(!isOpen() && len.load() <= i) return std::nullopt;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        sReallocLock.lock();
        elmt r = s[i];
        sReallocLock.unlock();
        return r;
    }




    /**
     * @brief Returns the address to the base value used by this BasePipe.
     *      Operations on this object are only thread-safe if .isOpen() returns false.
     * @return A pointer to the base object.
     */
    t *cpp() {
        return &s;
    }




    /**
     * @brief Returns the address to the base value used by this BasePipe.
     *      Operations on this object are only thread-safe if .isOpen() returns false.
     * @return A pointer to the base object.
     */
    t const *cpp() const {
        return &s;
    }
};