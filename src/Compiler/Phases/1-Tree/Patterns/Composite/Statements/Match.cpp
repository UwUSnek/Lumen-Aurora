#include "Match.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Match::getCategoryName(bool plural) const {
    return plural ? "match statements" : "match statement";
}

std::string cmp::Pattern_Sttm_Match::genDecoratedValue(bool article) const {
    return article
        ? "a Match statement"
        :   "Match statement"
    ;
}

ulong cmp::Pattern_Sttm_Match::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Match::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_MATCH),
        tk::Keyword(KEYWORD_ROUND_L),
        re::ANY_Expression(),
        tk::Keyword(KEYWORD_ROUND_R),
        tk::Keyword(KEYWORD_CURLY_L),
        op::Loop(1UL, re::Sttm_MatchCase()),
        tk::Keyword(KEYWORD_CURLY_R)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Match::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Match>();

    // Save data
    r->value = results[2];
    for(ulong i = 5; results[i]->isMatchCase(); ++i) {
        r->addChild(results[i]);
    }

    // Print debug info and return
    debug(console::cout << "found match statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}