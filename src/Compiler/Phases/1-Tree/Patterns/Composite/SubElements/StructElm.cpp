#include "StructElm.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include <memory>

std::string cmp::ST_StructElm::getCategoryName(bool plural) const {
    return plural ? "struct element" : "struct elements";
}

std::string cmp::Pattern_Elm_StructElm::genDecoratedValue(bool article) const {
    return article
        ? "a Struct Element Definition"
        :   "Struct Element Definition"
    ;
}

ulong cmp::Pattern_Elm_StructElm::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_StructElm::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Identifier(),
        op::Optional(1UL,
            tk::Keyword(ReservedTokenId::META_KEYWORD_ASSIGN)
            //FIXME expression
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_StructElm::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_StructElm>();

    r->name = results[0]->asIdentifier();
    //FIXME expression

    // Print debug info and return
    debug(cout << "Found struct element " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}