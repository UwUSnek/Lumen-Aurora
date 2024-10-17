#include "Type_Basic.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_BasicType::getCategoryName(bool plural) const {
    return plural ? "Type Path" : "Type Path";
}




void cmp::Pattern_Elm_Type_Basic::init() {
    __base_Pattern_Composite::__internal_init(
        re::Path(),
        op::Optional(
            tk::Keyword(ReservedTokenId::KEYWORD_PTR)
        )
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Type_Basic::generateData(std::vector<__base_ST*> const &results) const {
    ST_BasicType* r = new ST_BasicType;

    r->path = results[0]->asPath();
    r->isPointer = results.size() > 1;

    return dynamic_cast<__base_ST*>(r);
}




std::string cmp::Pattern_Elm_Type_Basic::genDecoratedValue() const {
    return "Type Path";
}


ulong cmp::Pattern_Elm_Type_Basic::getCertaintyThreshold() const {
    return 1;
}
