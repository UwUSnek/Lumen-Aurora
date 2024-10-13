#include "alias.hpp"
#include "Utils/errors.hpp"

#include "Compiler/TreePhase/Parsers/SubElements/path.hpp"

std::string cmp::ST_Alias::getCategoryName(bool plural) const {
    return plural ? "symbol alias definition" : "symbol alias definitions";
}




cmp::Pattern_Elm_Alias::Pattern_Elm_Alias() : __base_Pattern_Composite(
    tk::Keyword(ReservedTokenId::KEYWORD_ALIAS),
    re::Path(),
    tk::Keyword(ReservedTokenId::META_KEYWORD_AS),
    tk::Identifier(),
    tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
){}




cmp::__base_ST* cmp::Pattern_Elm_Alias::generateData(std::vector<__base_ST*> const &results) const {
    ST_Alias* r = new ST_Alias;
    r->original = results[1]->asPath();
    r->name     = results[3]->asIdentifier();
    debug(consoleLock.lock(); cout << "found alias " << r->name->s << "\n"; consoleLock.unlock();)
    return r;
}



