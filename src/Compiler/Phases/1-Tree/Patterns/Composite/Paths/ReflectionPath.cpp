#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "ReflectionPath.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Path_Reflection::getCategoryName(bool plural) const {
    return plural ? "reflection paths" : "reflection path";
}

std::string cmp::Pattern_Path_Reflection::genDecoratedValue(bool article) const {
    return article
        ? "a Reflection path"
        :   "Reflection path"
    ;
}

ulong cmp::Pattern_Path_Reflection::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Path_Reflection::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::ANY_Path(),
        tk::Keyword(META_KEYWORD_COLON),
        re::ANY_Path()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Path_Reflection::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Path_Reflection>();

    // Save value
    r->target = results[0];
    r->value = results[2];


    // Print debug info and return
    debug(console::cout << "found reflection path\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}