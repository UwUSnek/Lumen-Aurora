#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <chrono>








template<class t> struct __base_Pipe {
private:
    std::atomic<bool> _isOpen = true;




protected:
    t s;
    __base_Pipe(){}




public:
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
     * @brief Makes the thread sleep until the pipe closes or the provided function returns false.
     * @param task The task to run at each iteration. If this returns false, awaitClose returns instantly.
     *     The caller is responsible for error codes and messages. awaitClose provides no error handling.
     */
    void awaitClose(const std::function<bool()> &task) const { //NOSONAR
        while(isOpen()) {
            if(!task()) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
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