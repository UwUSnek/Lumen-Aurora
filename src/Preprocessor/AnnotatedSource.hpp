#pragma once
#include <string>
#include "Utils/Containers/VectorPipe.hpp"




namespace pre {

    /**
     * @brief A structure that contains informations about a single character
     */
    struct AnnotatedSourceMeta {
        ulong i;               // The original index of the character in the str buffer
        ulong l;               // The original   line number of the character in the str buffer (starts from 0)
        ulong c;               // The original column number of the character in the str buffer (starts from 0)
        ulong f;               // The index of the original file of the character in the str buffer


        AnnotatedSourceMeta() = delete;
        AnnotatedSourceMeta(const AnnotatedSourceMeta &meta) = default;
        AnnotatedSourceMeta(ulong _i, ulong _l, ulong _c, ulong _f) :
            i(_i),
            l(_l),
            c(_c),
            f(_f) {
        }
    };




    struct AnnotatedSourceElm {
        char c;
        AnnotatedSourceMeta meta;


        AnnotatedSourceElm() = delete;
        AnnotatedSourceElm(const AnnotatedSourceElm& elm) = default;
        AnnotatedSourceElm(const char _c, const AnnotatedSourceMeta& _meta) :
            c(_c),
            meta(_meta) {
        }

        char operator*() const {
            return c;
        }

        bool operator==(const char _c) const {
            return c == _c;
        }

        bool operator!=(const char _c) const {
            return c != _c;
        }

        //! Intentionally not explicit
        //! AnnotatedSourceElm implicitly should be convertible to chars to make the code more readable
        operator char() const { //NOSONAR
            return c;
        }
    };





    /**
     * @brief This structure contain source code that has gone through the cleanup phase.
     *      It retains informations about any removed portion of code.
     *      If the original file is available, it allows every character of the code to be traced back to its original position.
     */
    template<bool safeRealloc=true> struct AnnotatedSource : VectorPipe<AnnotatedSourceElm, safeRealloc> {


        std::string substr(ulong index, ulong len) {

            // Create result string with specified capacity and calculate end index
            std::string r;
            r.reserve(len);
            const ulong end = index + len;

            // Construct the substring
            for(ulong i = index; i < end; ++i) {
                const auto c = this->operator[](i);
                if(!c) return r;
                r += *c;
            }
            return r;
        }





        bool strcmp(ulong index, const char* str) {

            // For each character of the pipe (starting at the specified index) and the string (starting at index 0)
            for(ulong i = 0;; ++i) {
                const auto &entry = this->operator[](i + index);

                // Check if only the source code ended (no match)
                if(!entry) return false;

                // Check if desired string ended (match)
                if(str[i] == '\0') return true;

                // Compare characters (no match if different, keep checking otherwise)
                if(entry->c != str[i]) return false;
            }
        }




        bool strcmp(ulong index, const std::string &str) {
            //! Scoped lock is handled by the override
            return strcmp(index, str.c_str()); //NOSONAR
        }


        AnnotatedSource() = delete;
        using VectorPipe<AnnotatedSourceElm, safeRealloc>::VectorPipe;
    };
}