#include "ForHeaderRange.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_ForHeaderRange::getCategoryName(bool plural) const {
    return plural ? "range for headers" : "range for header";
}

std::string cmp::Pattern_Sttm_ForHeaderRange::genDecoratedValue(bool article) const {
    return article
        ? "a Range for header"
        :   "Range for header"
    ;
}

ulong cmp::Pattern_Sttm_ForHeaderRange::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Sttm_ForHeaderRange::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::Optional(2UL,
            re::ANY_Expression(),
            tk::Keyword(META_KEYWORD_TO)
        ),
        re::ANY_Expression(),
        tk::Keyword(META_KEYWORD_AS),
        tk::Identifier(),
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_ForHeaderRange::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_ForHeaderRange>();

    // Save data
    if(results[1]->isKeyword() && results[1]->asKeyword()->id == ReservedTokenId::META_KEYWORD_TO) {
        r->from = results[0];
        r->to   = results[2];
        r->name = results[4]->asIdentifier();
    }
    else {
        r->from = nullptr;
        r->to = results[0];
        r->name = results[2]->asIdentifier();
    }

    // Print debug info and return
    debug(console::cout << "found range for header\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}