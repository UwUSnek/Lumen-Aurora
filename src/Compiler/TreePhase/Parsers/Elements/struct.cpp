#include "struct.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Struct::getCategoryName(bool plural) const {
    return plural ? "struct definition" : "struct definitions";
}




void cmp::Pattern_Elm_Struct::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(ReservedTokenId::KEYWORD_STRUCT),
        tk::Identifier(),
        tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
        op::Optional(op::Loop(op::OneOf(
            re::Alias(),
            re::Enum(),
            re::Struct(),
            re::Namespace()
            //FIXME other possible elements
        ))),
        tk::Keyword(ReservedTokenId::KEYWORD_CURLY_R)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Struct::generateData(std::vector<__base_ST*> const &results) const {
    ST_Struct* r = new ST_Struct;

    // Set custom data
    r->name     = results[1]->asIdentifier();

    // Add child trees
    for(ulong i = 5; i < results.size() - 1; ++i) r->addChild(results[i]);

    // Print debug info and return
    debug((cout++ << "found struct " << r->name->s << "\n")--;)
    return dynamic_cast<__base_ST*>(r);
}




std::string cmp::Pattern_Elm_Struct::genDecoratedValue() const {
    return "Struct declaration";
}


ulong cmp::Pattern_Elm_Struct::getCertaintyThreshold() const {
    return 1;
}