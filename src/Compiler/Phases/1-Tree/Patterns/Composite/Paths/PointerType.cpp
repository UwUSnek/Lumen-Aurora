#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "PointerType.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Type_Pointer::getCategoryName(bool plural) const {
    return plural ? "pointer types" : "pointer type";
}

std::string cmp::Pattern_Type_Pointer::genDecoratedValue(bool article) const {
    return article
        ? "a Pointer type"
        :   "Pointer type"
    ;
}

ulong cmp::Pattern_Type_Pointer::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Type_Pointer::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::Pattern_Type_Basic(),
        op::Optional(1UL, tk::Keyword(KEYWORD_CONST)),
        tk::Keyword(META_KEYWORD_PTR)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Type_Pointer::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Type_Pointer>();

    // Save value
    r->baseType = results[0];
    r->isConst  = results.size() > 1;


    // Print debug info and return
    debug(console::cout << "found pointer type\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}