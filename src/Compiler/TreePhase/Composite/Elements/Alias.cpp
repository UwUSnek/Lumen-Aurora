#include "Alias.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Alias::getCategoryName(bool plural) const {
    return plural ? "symbol alias" : "symbol aliases";
}

std::string cmp::Pattern_Elm_Alias::genDecoratedValue(bool article) const {
    return article
        ? "a Symbol Alias Definition"
        :   "Symbol Alias Definition"
    ;
}

ulong cmp::Pattern_Elm_Alias::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Alias::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(ReservedTokenId::KEYWORD_ALIAS),
        re::Path(),
        tk::Keyword(ReservedTokenId::META_KEYWORD_AS),
        tk::Identifier(),
        tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Alias::generateData(std::vector<__base_ST*> const &results) const {
    ST_Alias* r = new ST_Alias;
    r->original = results[1]->asPath();
    r->name     = results[3]->asIdentifier();

    debug((cout++ << "found alias " << r->name->s << "\n")--;)
    return dynamic_cast<__base_ST*>(r);
}