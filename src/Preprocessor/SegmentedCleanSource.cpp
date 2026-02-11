#include "SegmentedCleanSource.hpp"
#include "Utils/format.hpp"
#include <optional>





std::string pre::SegmentedCleanSource::substr(ulong index, ulong len) {

    // Create result string with specified capacity and calculate end index
    std::string r;
    r.reserve(len);
    const ulong end = index + len;

    // Lock the pipe's reallocation muted and construct the substring
    std::scoped_lock lock(sReallocLock);
    for(ulong i = index; i < end; ++i) r += operator[](i)->c;
    return r;
}

// void pre::SegmentedCleanSource::operator+=(SegmentedCleanSource b) {
//     data += b.data;
// }

// std::optional<pre::CleanSourceElm> pre::SegmentedCleanSource::get(ulong index) {
//     return operator[](index);
// }

// std::optional<pre::CleanSourceElm> pre::SegmentedCleanSource::operator[](ulong index) {
//     return data[index];
// }

// std::optional<char> pre::SegmentedCleanSource::str(ulong index) {
//     auto r = data[index];
//     if(r.has_value()) return r->c;
//     else return std::nullopt;
// }

// std::optional<pre::CleanSourceMeta> pre::SegmentedCleanSource::meta(ulong index) {
//     auto r = data[index];
//     if(r.has_value()) return r->meta;
//     else return std::nullopt;
// }