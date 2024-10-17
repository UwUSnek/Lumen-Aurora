#include "__base_Pattern.hpp"
#include "Utils/ansi.hpp"
#include "PatternGenerators.hpp"






// Pattern isType and asType functions
#define X(type, name) \
    const cmp::type *cmp::__base_Pattern::as##name() const { return dynamic_cast<const type*>(this); } \
    /**/  cmp::type *cmp::__base_Pattern::as##name()       { return dynamic_cast<      type*>(this); } \
    /**/       bool  cmp::__base_Pattern::is##name() const { return dynamic_cast<const type*>(this); } \
    /**/       bool  cmp::__base_Pattern::is##name()       { return dynamic_cast<      type*>(this); }
LIST_PATTERN_OPERATOR_TYPES_NAMES
LIST_PATTERN_BASES_TYPES_NAMES
LIST_PATTERN_TOKENS_TYPES_NAMES
#undef X




// Pattern singletons
namespace cmp {
    #define X(type, name) \
        type *re::__internal_cache_##name = nullptr;
    LIST_PATTERN_ELM_TYPES_NAMES
    #undef X
}








debug(
    void cmp::printPatternElmInfo(__base_Pattern* p, int indent) {
        if(p->isOneOf()) {
            __Pattern_Operator_OneOf *p2 = p->asOneOf();
            cout << __internal_repeat(ansi::bright_black + "│ " + ansi::reset, indent) << p << " (" << p2 << " as OneOf)\n";
            for(ulong i = 0; i < p2->v.size(); ++i){
                printPatternElmInfo(p2->v[i], indent + 1);
            }
        }
        else if(p->isLoop()) {
            __Pattern_Operator_Loop *p2 = p->asLoop();
            cout << __internal_repeat(ansi::bright_black + "│ " + ansi::reset, indent) << p << " (" << p2 << " as Loop)\n";
            for(ulong i = 0; i < p2->v.size(); ++i){
                printPatternElmInfo(p2->v[i], indent + 1);
            }
        }
        else {
            cout << __internal_repeat(ansi::bright_black + "│ " + ansi::reset, indent) << p << "\n";
        }
    }
)