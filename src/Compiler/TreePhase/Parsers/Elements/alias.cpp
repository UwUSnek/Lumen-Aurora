#include "alias.hpp"
#include "Utils/errors.hpp"

#include "Compiler/TreePhase/Parsers/SubElements/path.hpp"

std::string cmp::ST_Alias::getCategoryName(bool plural) const {
    return plural ? "symbol alias definition" : "symbol alias definitions";
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
    debug(consoleLock.lock(); cout << "found alias " << r->name->s << "\n"; consoleLock.unlock();)
    return dynamic_cast<__base_ST*>(r);
}



