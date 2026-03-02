#include "OptionalParameter.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>

std::string cmp::ST_OptionalParameter::getCategoryName(bool plural) const {
    return plural ? "optional parameters" : "optional parameter";
}

std::string cmp::Pattern_Elm_OptionalParameter::genDecoratedValue(bool article) const {
    return article
        ? "a Optional Parameter"
        :   "Optional Parameter"
    ;
}

ulong cmp::Pattern_Elm_OptionalParameter::getCertaintyThreshold() const {
    return 1;
}





void cmp::Pattern_Elm_OptionalParameter::init() {
    using enum ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::BasicParameter(),
        tk::Keyword(META_KEYWORD_ASSIGN),
        re::ANY_Expression()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_OptionalParameter::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_OptionalParameter>();

    r->type = results[0]->asBasicParameter()->type;
    r->name = results[0]->asBasicParameter()->name;
    r->defaultValue = results[2];

    // Print debug info and return
    debug(console::cout << "Found optional parameter " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}