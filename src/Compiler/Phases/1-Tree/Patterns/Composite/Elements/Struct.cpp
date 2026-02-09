#include "Struct.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"

std::string cmp::ST_Struct::getCategoryName(bool plural) const {
    return plural ? "struct definition" : "struct definitions";
}

std::string cmp::Pattern_Elm_Struct::genDecoratedValue(bool article) const {
    return article
        ? "a Struct Definition"
        :   "Struct Definition"
    ;
}

ulong cmp::Pattern_Elm_Struct::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Struct::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_STRUCT),
        tk::Identifier(),
        tk::Keyword(KEYWORD_CURLY_L),
        op::Optional((ulong)-1, op::Loop((ulong)-1, op::OneOf(
            re::StructElmCluster(),
            re::Import(),
            re::Export(),
            re::Alias(),
            re::Enum(),
            re::Struct(),
            re::Namespace()
            //FIXME other possible elements
        ))),
        tk::Keyword(KEYWORD_CURLY_R)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Struct::generateData(std::vector<__base_ST*> const &results) const {
    auto* r = new ST_Struct;

    // Set custom data
    r->name = results[1]->asIdentifier();

    // Add child trees
    for(ulong i = 5; i < results.size() - 1; ++i) r->addChild(results[i]);

    // Print debug info and return
    debug((cout++ << "found struct " << r->name->s << "\n")--;)
    return dynamic_cast<__base_ST*>(r);
}