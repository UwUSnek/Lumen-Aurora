#pragma once




/**
 * @brief An exception thrown by subphase threads when they encounter a parsing error.
 * This is exclusively used to safely unwind their stack and let the main thread join subphase threads without generating additional errors or UB.
 */
struct FatalErrorException {
    int exitCode;
    explicit FatalErrorException(int code) noexcept : exitCode(code) {}
};