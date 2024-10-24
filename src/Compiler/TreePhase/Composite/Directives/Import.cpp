#include "Import.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Import::getCategoryName(bool plural) const {
    return plural ? "import directive" : "importe directives";
}

std::string cmp::Pattern_Elm_Import::genDecoratedValue(bool article) const {
    return article
        ? "an Import directive"
        :    "Import directive"
    ;
}

ulong cmp::Pattern_Elm_Import::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Import::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(ReservedTokenId::KEYWORD_IMPORT),
        re::Path(),
        tk::Keyword(ReservedTokenId::META_KEYWORD_AS),
        tk::Identifier(),
        tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Import::generateData(std::vector<__base_ST*> const &results) const {
    ST_Import* r = new ST_Import;
    r->original = results[1]->asPath();
    r->name     = results[3]->asIdentifier();

    debug((cout++ << "found import directive\n")--;)
    return dynamic_cast<__base_ST*>(r);
}