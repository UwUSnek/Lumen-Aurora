#include "SegmentedCleanSource.hpp"
#include <optional>





std::string pre::SegmentedCleanSource::substr(ulong index, ulong len) {

    // Create result string with specified capacity and calculate end index
    std::string r;
    r.reserve(len);
    const ulong end = index + len;

    // Lock the pipe's reallocation mutex and construct the substring
    auto lock = this->scoped_lock();
    for(ulong i = index; i < end; ++i) r += operator[](i)->c;
    return r;
}




bool pre::SegmentedCleanSource::strcmp(ulong index, const char* str) {

    // Lock the pipe's reallocation mutex
    auto lock = this->scoped_lock();

    // For each character of the pipe (starting at the specified index) and the string (starting at index 0)
    for(ulong i = 0;; ++i) {
        const auto &entry = operator[](i + index);
        bool data_ended = !entry.has_value();
        bool str_ended = (str[i] == '\0');

        // Check if strings ended together (match)
        if(data_ended && str_ended) return true;

        // Check if only one string ended (no match)
        if(data_ended || str_ended) return false;

        // Compare characters (no match if different, keep checking otherwise)
        if(entry->c != str[i]) return false;
    }
}




bool pre::SegmentedCleanSource::strcmp(ulong index, const std::string &str) {
    //! Scoped lock is created by the override
    return strcmp(index, str.c_str()); //NOSONAR
}