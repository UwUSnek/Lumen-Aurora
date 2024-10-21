#include "StructElmCluster.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"
#include "StructElm.hpp"

std::string cmp::ST_StructElmCluster::getCategoryName(bool plural) const {
    return plural ? "struct element" : "struct elements";
}

std::string cmp::Pattern_Elm_StructElmCluster::genDecoratedValue(bool article) const {
    return article
        ? "a Struct Element Cluster Definition"
        :   "Struct Element Cluster Definition"
    ;
}

ulong cmp::Pattern_Elm_StructElmCluster::getCertaintyThreshold() const {
    return 2; //! Type + element
}









void cmp::Pattern_Elm_StructElmCluster::init() {
    __base_Pattern_Composite::__internal_init(
        re::Type(),
        re::StructElm(),
        op::Optional(op::Loop(
            tk::Keyword(ReservedTokenId::KEYWORD_COMMA),
            re::StructElm()
        )),
        tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_StructElmCluster::generateData(std::vector<__base_ST*> const &results) const {
    ST_StructElmCluster* r = new ST_StructElmCluster();

    // Save type
    r->type = results[0]->asType();

    // Save individual elements
    for(ulong i = 1; i < results.size(); i += 2) {
        r->elms.push_back(dynamic_cast<ST_StructElm*>(results[i]));
    }

    // Print debug info and return
    return dynamic_cast<__base_ST*>(r);
}