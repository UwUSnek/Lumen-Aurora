#include "TreePatterns.hpp"
#include "Utils/ansi.hpp"
#include "Parsers/Elements/alias.hpp"
#include "Parsers/Elements/enum.hpp"
#include "Parsers/Elements/namespace.hpp"
#include "Parsers/Elements/struct.hpp"
#include "Parsers/SubElements/path.hpp"

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

void cmp::Pattern_Elm_Module::init() {
    __base_Pattern_Composite::__internal_init(
        op::Optional(op::Loop(op::OneOf(
            re::Alias(),
            re::Namespace(),
            re::Struct(),
            re::Enum()
        )))
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Module::generateData(std::vector<__base_ST*> const &results) const {
    ST_Module* r = new ST_Module;
    for(ulong i = 0; i < results.size(); ++i) {
        r->addChild(results[i]);
    }
    return dynamic_cast<__base_ST*>(r);
}


ulong cmp::Pattern_Elm_Module::getCertaintyThreshold() const {
    return (ulong)-1;
}