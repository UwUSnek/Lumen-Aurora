#include "Type.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Type::getCategoryName(bool plural) const {
    return plural ? "type" : "types";
}

std::string cmp::Pattern_Elm_Type::genDecoratedValue(bool article) const {
    return article
        ? "a Type Path"
        :   "Type Path"
    ;
}

ulong cmp::Pattern_Elm_Type::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Type::init() {
    //FIXME incorrect error message. The type is considered "incomplete", when in reality what's incomplete is whatever is's part of.
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            re::BasicType(),
            re::FunctionType()
        )
    );
    //FIXME print all possible expected elements when a OneOf fails, instead of just the first one
}




cmp::__base_ST* cmp::Pattern_Elm_Type::generateData(std::vector<__base_ST*> const &results) const {
    ST_Type* r = results[0]->asType();

    // Return
    return dynamic_cast<__base_ST*>(r);
}