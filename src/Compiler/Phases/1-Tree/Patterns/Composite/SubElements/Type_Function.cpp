#include "Type_Function.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include <memory>

std::string cmp::ST_FunctionType::getCategoryName(bool plural) const {
    return plural ? "function type" : "function types";
}

std::string cmp::Pattern_Elm_Type_Function::genDecoratedValue(bool article) const {
    return "" debug("Debug:TypePathFunction");
}

ulong cmp::Pattern_Elm_Type_Function::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Type_Function::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_FN),
        tk::Keyword(KEYWORD_ROUND_L),
        op::Optional((ulong)-1,
            re::Type(),
            op::Optional((ulong)-1, op::Loop(1,
                tk::Keyword(KEYWORD_COMMA),
                re::Type()
            ))
        ),
        tk::Keyword(KEYWORD_ROUND_R),
        op::Optional((ulong)-1, op::Loop((ulong)-1,
            tk::Keyword(KEYWORD_PTR)
        ))
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_Type_Function::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_FunctionType>();

    // Set return type and isPointer
    r->retType = results[0]->asType();

    // Save parameter types
    ulong i;
    for(i = 1;; ++i) {
        if(results[i]->isKeyword() && results[i]->asKeyword()->id == ReservedTokenId::KEYWORD_ROUND_R) break;
        if(results[i]->isType()) r->argTypes.push_back(results[i]->asType());
    }

    // Save pointer number
    r->pointerNum = results.size() - i - 1 /*don't count the } character*/;

    // Return
    return std::dynamic_pointer_cast<__base_ST>(r);
}