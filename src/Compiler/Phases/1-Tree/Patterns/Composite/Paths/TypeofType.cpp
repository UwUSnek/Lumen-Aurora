#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "TypeofType.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Type_Typeof::getCategoryName(bool plural) const {
    return plural ? "typeof specifiers" : "typeof specifier";
}

std::string cmp::Pattern_Type_Typeof::genDecoratedValue(bool article) const {
    return article
        ? "a Typeof specifier"
        :   "Typeof specifier"
    ;
}

ulong cmp::Pattern_Type_Typeof::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Type_Typeof::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_TYPEOF),
        tk::Keyword(KEYWORD_ROUND_L),
        re::ANY_Expression(),
        tk::Keyword(KEYWORD_ROUND_R),
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Type_Typeof::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Type_Typeof>();

    // Save value
    r->value = results[2];

    // Print debug info and return
    debug(console::cout << "found typeof specifier\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}