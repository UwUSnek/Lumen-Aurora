#include "path.hpp"
#include "Utils/errors.hpp"

std::string cmp::ST_Sub_Path::getCategoryName(bool plural) const {
    return plural ? "symbol path" : "symbol paths";
} //TODO check if plural is needed




void cmp::Pattern_Elm_Path::init(){
    __base_Pattern_Composite::__internal_init(
        tk::Identifier(),
        op::Optional(op::Loop(
            tk::Keyword(ReservedTokenId::KEYWORD_DOT),
            tk::Identifier()
        ))
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Path::generateData(std::vector<__base_ST*> const &results) const {
    ST_Sub_Path* r = new ST_Sub_Path;

    // Set all the identifiers (start from 0 and skip . keyword every iteration)
    for(ulong i = 0; i < results.size(); i += 2) {
        r->idList.push_back(results[i]->asIdentifier()->s);
    }

    // Return
    return dynamic_cast<__base_ST*>(r);
}




std::string cmp::Pattern_Elm_Path::genDecoratedValue() const {
    return "Symbol Path";
}