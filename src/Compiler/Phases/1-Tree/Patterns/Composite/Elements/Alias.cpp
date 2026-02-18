#include "Alias.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include <memory>

std::string cmp::ST_Alias::getCategoryName(bool plural) const {
    return plural ? "symbol aliases" : "symbol alias";
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
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_ALIAS),
        re::Path(),
        tk::Keyword(META_KEYWORD_AS),
        tk::Identifier(),
        tk::Keyword(KEYWORD_SEMICOLON)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_Alias::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Alias>();
    r->original = results[1]->asPath();
    r->name     = results[3]->asIdentifier();

    debug(cout << "found alias " << r->name->s << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}