#include "Enum.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Enum::getCategoryName(bool plural) const {
    return plural ? "enum definition" : "enum definitions";
}

std::string cmp::Pattern_Elm_Enum::genDecoratedValue(bool article) const {
    return article
        ? "an Enum definition"
        :    "Enum definition"
    ;
}

ulong   cmp::Pattern_Elm_Enum::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Elm_Enum::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(ReservedTokenId::KEYWORD_ENUM),
        tk::Identifier(),
        tk::Keyword(ReservedTokenId::META_KEYWORD_COLON),
        re::Type(),
        tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
        op::Optional(op::Loop(op::OneOf(
            re::EnumElm(),
            re::Alias(),
            re::Enum(),
            re::Struct(),
            re::Namespace()
            //FIXME other possible elements
        ))),
        tk::Keyword(ReservedTokenId::KEYWORD_CURLY_R)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Enum::generateData(std::vector<__base_ST*> const &results) const {
    ST_Enum* r = new ST_Enum;

    // Set custom data
    r->name     = results[1]->asIdentifier();
    r->baseType = results[3]->asType();

    // Add child trees
    for(ulong i = 5; i < results.size() - 1; ++i) r->addChild(results[i]);

    // Print debug info and return
    debug((cout++ << "found enum " << r->name->s << "\n")--;)
    return dynamic_cast<__base_ST*>(r);
}