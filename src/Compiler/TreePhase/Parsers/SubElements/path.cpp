#include "path.hpp"
#include "Utils/errors.hpp"

std::string cmp::ST_Sub_Path::getCategoryName(bool plural) const {
    return plural ? "symbol path" : "symbol paths";
} //TODO check if plural is needed




cmp::Pattern_Elm_Path::Pattern_Elm_Path() : __base_Pattern_Composite(
    tk::Identifier(),
    op::Loop(
        tk::Keyword(ReservedTokenId::KEYWORD_DOT),
        tk::Identifier()
    )
){}




cmp::__base_ST* cmp::Pattern_Elm_Path::generateData(std::vector<__base_ST*> const &results) const {
    ST_Sub_Path* r = new ST_Sub_Path;
    for(ulong i = 0; i < results.size(); i += 2) {
        r->idList.push_back(results[i]->asIdentifier()->s);
    }
    return r;
}