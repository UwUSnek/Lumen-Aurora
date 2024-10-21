#include "EnumElm.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_EnumElm::getCategoryName(bool plural) const {
    return plural ? "enum element" : "enum elements";
}

std::string cmp::Pattern_Elm_EnumElm::genDecoratedValue(bool article) const {
    return article
        ? "an Enum Element Definition"
        :    "Enum Element Definition"
    ;
}

ulong cmp::Pattern_Elm_EnumElm::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_EnumElm::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Identifier(),
        op::Optional(
            tk::Keyword(ReservedTokenId::META_KEYWORD_ASSIGN)
            //FIXME expression
        ),
        tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_EnumElm::generateData(std::vector<__base_ST*> const &results) const {
    ST_EnumElm* r = new ST_EnumElm();

    r->name = results[0]->asIdentifier()->s;

    // Print debug info and return
    debug((cout++ << "Found enum element " << r->name << "\n")--;)
    return dynamic_cast<__base_ST*>(r);
}