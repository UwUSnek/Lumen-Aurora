#include "EnumElm.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>

std::string cmp::ST_EnumElm::getCategoryName(bool plural) const {
    return plural ? "enum elements" : "enum element";
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
        op::Optional(1UL,
            tk::Keyword(ReservedTokenId::META_KEYWORD_ASSIGN)
            //FIXME expression
        ),
        tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_EnumElm::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_EnumElm>();

    r->name = results[0]->asIdentifier();

    // Print debug info and return
    debug(console::cout << "Found enum element " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}