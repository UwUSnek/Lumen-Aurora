#pragma once
#include <memory>


template<typename T>
using ptr = std::shared_ptr<T>;



template<typename T, typename... Args>
ptr<T> newptr(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}