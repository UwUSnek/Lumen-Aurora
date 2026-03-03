#include "WhileHeader.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_WhileHeader::getCategoryName(bool plural) const {
    return plural ? "while headers" : "while header";
}

std::string cmp::Pattern_Sttm_WhileHeader::genDecoratedValue(bool article) const {
    return article
        ? "a While header"
        :   "While header"
    ;
}

ulong cmp::Pattern_Sttm_WhileHeader::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_WhileHeader::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_WHILE),
        tk::Keyword(KEYWORD_ROUND_L),
        re::ANY_Expression(),
        tk::Keyword(KEYWORD_ROUND_R),
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_WhileHeader::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_WhileHeader>();

    // Save data
    r->condition = results[2];

    // Print debug info and return
    debug(console::cout << "found while header\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}