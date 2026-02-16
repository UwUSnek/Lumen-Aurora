#include "Type_Basic.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include <memory>

std::string cmp::ST_BasicType::getCategoryName(bool plural) const {
    return plural ? "basic type" : "basic types";
}

std::string cmp::Pattern_Elm_Type_Basic::genDecoratedValue(bool article) const {
    return article
        ? "a Basic Type Path"
        :   "Basic Type Path"
    ;}

ulong cmp::Pattern_Elm_Type_Basic::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Type_Basic::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::Path(),
        op::Optional((ulong)-1, op::Loop((ulong)-1,
            tk::Keyword(KEYWORD_PTR)
        ))
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_Type_Basic::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_BasicType>();

    r->path = results[0]->asPath();
    r->pointerNum = results.size() - 1;

    return std::dynamic_pointer_cast<__base_ST>(r);
}