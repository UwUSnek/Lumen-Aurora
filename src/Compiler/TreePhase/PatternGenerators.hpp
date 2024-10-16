#pragma once
#include "SourceTree.hpp"
#include "Parsers/Elements/alias.hpp"
#include "Parsers/Elements/enum.hpp"
#include "Parsers/Elements/namespace.hpp"
#include "Parsers/Elements/struct.hpp"
#include "Parsers/SubElements/path.hpp"








namespace cmp {
    namespace re {
        // Custom new function that allows the macro to create elements before their types are fully defined
        template<class t> t* __internal_forwardNew() {
            return new t();
        }


        // Custom init wrapper that allows the macro to initialize elements before their types are fully defined
        template<class t, class ...u> void __internal_forwardInit(t* ptr, u... subPatterns) {
            ptr->init(subPatterns...);
        }


        // Pattern singletons and value generators
        // Usage: re::<name>(<patterns>)
        //! Placement new prevents circular dependencies between patterns
        #define X(type, name)                                                                             \
            extern type *__internal_cache_##name;                                                         \
            template<class ...t> type *name(t... subPatterns) {                                           \
                if(!__internal_cache_##name) {                                                            \
                    __internal_cache_##name = __internal_forwardNew<type>();                              \
                    debug((cout++ << "allocated   " << __internal_cache_##name << " | "#name << "\n")--;) \
                    __internal_forwardInit<type>(__internal_cache_##name);                                \
                    debug((cout++ << "initialized " << __internal_cache_##name << " | "#name << "\n")--;) \
                }                                                                                         \
                else {                                                                                    \
                    debug((cout++ << "found       " << __internal_cache_##name << " | "#name << "\n")--;) \
                }                                                                                         \
                return __internal_cache_##name;                                                           \
            }
        LIST_PATTERN_ELM_TYPES_NAMES
        #undef X
    }




    namespace op {
        // Value generators for pattern operators (they don't need singletons and are stored in a different namespace)
        // Usage: op::<name>(<patterns>)
        #define X(type, name)                                                   \
            template<class ...t> type *name(t... subPatterns) {                 \
                type* r = re::__internal_forwardNew<type>();                    \
                re::__internal_forwardInit<type, t...>(r, subPatterns...);      \
                debug((cout++ << "created     " << r << " | "#name << "\n")--;) \
                return r;                                                       \
            }
        LIST_PATTERN_OPERATOR_TYPES_NAMES
        #undef X
    }



    namespace tk {
        // Value generators for token operators (they don't need singletons and are stored in a different namespace)
        // Usage: tk::<name>(<expected value>?)
        #define X(type, name)                                                   \
            template<class ...t> type *name(t... expectedValue) {               \
                type* r = re::__internal_forwardNew<type>();                    \
                re::__internal_forwardInit<type, t...>(r, expectedValue...);    \
                debug((cout++ << "created     " << r << " | "#name << "\n")--;) \
                return r;                                                       \
            }
        LIST_PATTERN_TOKENS_TYPES_NAMES
        #undef X
    }
}