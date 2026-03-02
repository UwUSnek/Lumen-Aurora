#include "ParameterPack.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>

std::string cmp::ST_ParameterPack::getCategoryName(bool plural) const {
    return plural ? "parameter packs" : "parameter pack";
}

std::string cmp::Pattern_Elm_ParameterPack::genDecoratedValue(bool article) const {
    return article
        ? "a Parameter Pack"
        :   "Parameter Pack"
    ;
}

ulong cmp::Pattern_Elm_ParameterPack::getCertaintyThreshold() const {
    return 1;
}





void cmp::Pattern_Elm_ParameterPack::init() {
    using enum ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::BasicParameter(),
        tk::Keyword(META_KEYWORD_ELLIPSIS)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_ParameterPack::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_ParameterPack>();

    r->type = results[0]->asBasicParameter()->type;
    r->name = results[0]->asBasicParameter()->name;

    // Print debug info and return
    debug(console::cout << "Found parameter pack " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}