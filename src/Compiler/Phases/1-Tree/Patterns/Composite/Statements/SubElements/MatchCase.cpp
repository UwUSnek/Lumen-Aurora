#include "MatchCase.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Sub_MatchCase::getCategoryName(bool plural) const {
    return plural ? "match cases" : "match case";
}

std::string cmp::Pattern_Sttm_Sub_MatchCase::genDecoratedValue(bool article) const {
    return article
        ? "a Match Case"
        :   "Match Case"
    ;
}

ulong cmp::Pattern_Sttm_Sub_MatchCase::getCertaintyThreshold() const {

    //! Match cases are only expected in match statements, and they are also the only allowed composite element in there.
    //! Certainty threshold 1 works just fine for this case.
    return 1;
}








void cmp::Pattern_Sttm_Sub_MatchCase::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::ANY_Expression(),
        op::Optional(1UL,
            tk::Keyword(META_KEYWORD_TO),
            re::ANY_Expression()
        ),
        op::Optional(
            tk::Keyword(KEYWORD_COMMA),
            re::ANY_Expression(),
            op::Optional(1UL,
                tk::Keyword(META_KEYWORD_TO),
                re::ANY_Expression()
            )
        ),
        tk::Keyword(META_KEYWORD_COLON),
        re::ANY_Statement()
    );
    //FIXME add error paths - check other declarations etc.
    //FIXME might need to automate a "check all known declarations" or something. fallback to "unexpected token" if no node is recognized beyond doubt
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Sub_MatchCase::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Sub_MatchCase>();
    ulong i = 0;



    // Save data
    while(!(results[i]->isKeyword() && results[i]->asKeyword()->id == ReservedTokenId::META_KEYWORD_COLON)) {
        if(results[i + 1]->isKeyword() && results[i + 1]->asKeyword()->id == ReservedTokenId::META_KEYWORD_TO) {
            r->ranges.emplace_back(results[i], results[i + 2]);
            i += 3;
        }
        else {
            r->ranges.emplace_back(results[i], nullptr);
            i++;
        }
        if(results[i]->isKeyword() && results[i]->asKeyword()->id == ReservedTokenId::KEYWORD_COMMA) {
            ++i;
        }
    }
    ++i;
    r->statement = results[i];



    // Print debug info and return
    debug(console::cout << "found match case\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}