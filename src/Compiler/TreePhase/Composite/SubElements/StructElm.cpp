#include "StructElm.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

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
        op::Optional(
            tk::Keyword(ReservedTokenId::META_KEYWORD_ASSIGN)
            //FIXME expression
        )
    );
}




cmp::__base_ST* cmp::Pattern_Elm_StructElm::generateData(std::vector<__base_ST*> const &results) const {
    ST_StructElm* r = new ST_StructElm();

    r->name = results[0]->asIdentifier()->s;
    //FIXME expression

    // Print debug info and return
    debug((cout++ << "Found struct element " << r->name << "\n")--;)
    return dynamic_cast<__base_ST*>(r);
}