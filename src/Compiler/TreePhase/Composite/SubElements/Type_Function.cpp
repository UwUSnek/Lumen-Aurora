#include "Type_Function.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_FunctionType::getCategoryName(bool plural) const { return plural ? "function type" : "function types"; }
std::string cmp::Pattern_Elm_Type_Function::genDecoratedValue() const { return "Function Type"; }
ulong   cmp::Pattern_Elm_Type_Function::getCertaintyThreshold() const { return 1; }








void cmp::Pattern_Elm_Type_Function::init() {
    __base_Pattern_Composite::__internal_init(
        re::Path(),
        op::Optional(
            tk::Keyword(ReservedTokenId::KEYWORD_PTR) //FIXME save the number of pointers instead. int@@@@@ is valid
        ),
        tk::Keyword(ReservedTokenId::KEYWORD_ROUND_L),
        op::Optional(
            re::Type(),
            op::Optional(op::Loop(
                tk::Keyword(ReservedTokenId::KEYWORD_COMMA),
                re::Type()
            ))
        ),
        tk::Keyword(ReservedTokenId::KEYWORD_ROUND_R)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Type_Function::generateData(std::vector<__base_ST*> const &results) const {
    ST_FunctionType* r = new ST_FunctionType;

    // Set return type and isPointer
    r->retType = results[0]->asType();
    if(results[0]->isKeyword()) r->isPointer = results[0]->asKeyword()->id == ReservedTokenId::KEYWORD_PTR; //FIXME save the number of pointers instead. int@@@@@ is valid

    // Save parameter types
    for(ulong i = 2 + r->isPointer; i < results.size() - 1; ++i) {
        r->argTypes.push_back(results[i]->asType());
    }

    // Return
    return dynamic_cast<__base_ST*>(r);
}