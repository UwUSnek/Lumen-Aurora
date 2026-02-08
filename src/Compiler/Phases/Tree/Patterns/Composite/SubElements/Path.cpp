#include "Path.hpp"
#include "Compiler/Phases/Tree/PatternGenerators.hpp"

std::string cmp::ST_Sub_Path::getCategoryName(bool plural) const {
    return plural ? "symbol path" : "symbol paths";
}

std::string cmp::Pattern_Elm_Path::genDecoratedValue(bool article) const {
    return article
        ? "a Symbol Path"
        :   "Symbol Path"
    ;
}

ulong cmp::Pattern_Elm_Path::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Path::init(){
    __base_Pattern_Composite::__internal_init(
        tk::Identifier(),
        op::Optional((ulong)-1, op::Loop(1,
            tk::Keyword(ReservedTokenId::KEYWORD_DOT), //FIXME add reflection paths
            tk::Identifier()
        ))
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Path::generateData(std::vector<__base_ST*> const &results) const {
    auto* r = new ST_Sub_Path;

    // Set all the identifiers (start from 0 and skip . keyword every iteration)
    for(ulong i = 0; i < results.size(); i += 2) {
        if(results[i]->isIdentifier()) r->idList.push_back(results[i]->asIdentifier()->s);
    }

    // Return
    return dynamic_cast<__base_ST*>(r);
}