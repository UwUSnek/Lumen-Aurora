#include "BasicParameter.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>

std::string cmp::ST_BasicParameter::getCategoryName(bool plural) const {
    return plural ? "basic parameters" : "basic parameter";
}

std::string cmp::Pattern_Elm_BasicParameter::genDecoratedValue(bool article) const {
    return article
        ? "a Basic Parameter"
        :   "Basic Parameter"
    ;
}

ulong cmp::Pattern_Elm_BasicParameter::getCertaintyThreshold() const {
    return 1;
}





void cmp::Pattern_Elm_BasicParameter::init() {
    using enum ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::Type(),
        tk::Identifier()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_BasicParameter::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_BasicParameter>();

    r->type = results[0]->asType();
    r->name = results[1]->asIdentifier();

    // Print debug info and return
    debug(console::cout << "Found basic parameter " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}