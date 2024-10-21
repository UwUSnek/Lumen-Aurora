#include "Type_Basic.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_BasicType::getCategoryName(bool plural) const {
    return plural ? "basic type" : "basic types";
}

std::string cmp::Pattern_Elm_Type_Basic::genDecoratedValue(bool article) const {
    return "" debug("Debug:TypePathBasic");
}

ulong cmp::Pattern_Elm_Type_Basic::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Type_Basic::init() {
    __base_Pattern_Composite::__internal_init(
        re::Path(),
        op::Optional(op::Loop(
            tk::Keyword(ReservedTokenId::KEYWORD_PTR)
        ))
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Type_Basic::generateData(std::vector<__base_ST*> const &results) const {
    ST_BasicType* r = new ST_BasicType;

    r->path = results[0]->asPath();
    r->pointerNum = results.size() - 1;

    return dynamic_cast<__base_ST*>(r);
}