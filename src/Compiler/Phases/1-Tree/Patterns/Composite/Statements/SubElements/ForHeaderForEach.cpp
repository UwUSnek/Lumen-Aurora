#include "ForHeaderForEach.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_ForHeaderForEach::getCategoryName(bool plural) const {
    return plural ? "for-each for headers" : "for-each for header";
}

std::string cmp::Pattern_Sttm_ForHeaderForEach::genDecoratedValue(bool article) const {
    return article
        ? "a For-each for header"
        :   "For-each for header"
    ;
}

ulong cmp::Pattern_Sttm_ForHeaderForEach::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Sttm_ForHeaderForEach::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Identifier(),
        tk::Keyword(META_KEYWORD_IN),
        re::ANY_Expression()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_ForHeaderForEach::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_ForHeaderForEach>();

    // Save data
    r->name  = results[0]->asIdentifier();
    r->value = results[2];


    // Print debug info and return
    debug(console::cout << "found for-each for header\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}