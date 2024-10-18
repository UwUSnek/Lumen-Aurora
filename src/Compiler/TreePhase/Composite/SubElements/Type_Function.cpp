#include "Type_Function.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_FunctionType::getCategoryName(bool plural) const { return plural ? "function type" : "function types"; }
std::string cmp::Pattern_Elm_Type_Function::genDecoratedValue() const { return "Function Type"; }
ulong   cmp::Pattern_Elm_Type_Function::getCertaintyThreshold() const { return 1; }








void cmp::Pattern_Elm_Type_Function::init() {
    __base_Pattern_Composite::__internal_init(
        re::Type(),
        tk::Keyword(ReservedTokenId::KEYWORD_ROUND_L),
        op::Optional(
            re::Type(),
            op::Optional(op::Loop(
                tk::Keyword(ReservedTokenId::KEYWORD_COMMA),
                re::Type()
            ))
        ),
        tk::Keyword(ReservedTokenId::KEYWORD_ROUND_R),
        op::Optional(op::Loop(
            tk::Keyword(ReservedTokenId::KEYWORD_PTR)
        ))
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Type_Function::generateData(std::vector<__base_ST*> const &results) const {
    ST_FunctionType* r = new ST_FunctionType;

    // Set return type and isPointer
    r->retType = results[0]->asType();

    // Save parameter types
    ulong i;
    for(i = 1;; ++i) {
        if(results[i]->isKeyword() && results[i]->asKeyword()->id == ReservedTokenId::KEYWORD_ROUND_R) break;
        if(results[i]->isType()) r->argTypes.push_back(results[i]->asType());
    }

    // Save pointer number
    r->pointerNum = results.size() - i - (1 /*don't count the } character*/);

    // Return
    return dynamic_cast<__base_ST*>(r);
}