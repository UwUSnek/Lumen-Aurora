#pragma once
#include "SourceTree.hpp"


#include "Composite/Module.hpp"

#include "Composite/Elements/Alias.hpp"
#include "Composite/Elements/Namespace.hpp"

#include "Composite/Elements/Enum.hpp"
#include "Composite/SubElements/EnumElm.hpp"

#include "Composite/Elements/Struct.hpp"

#include "Composite/SubElements/Path.hpp"
#include "Composite/SubElements/Type.hpp"
#include "Composite/SubElements/Type_Basic.hpp"
#include "Composite/SubElements/Type_Function.hpp"

#include "Operators/OperatorLoop.hpp"
#include "Operators/OperatorOneOf.hpp"
#include "Operators/OperatorOptional.hpp"

#include "Tokens/Keyword.hpp"
#include "Tokens/Identifier.hpp"
#include "Tokens/Literal.hpp"








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