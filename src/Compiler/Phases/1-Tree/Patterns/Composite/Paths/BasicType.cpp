#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "BasicType.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Type_Basic::getCategoryName(bool plural) const {
    return plural ? "basic types" : "basic type";
}

std::string cmp::Pattern_Type_Basic::genDecoratedValue(bool article) const {
    return article
        ? "a Basic type"
        :   "Basic type"
    ;
}

ulong cmp::Pattern_Type_Basic::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Type_Basic::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::ANY_Path()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Type_Basic::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Type_Basic>();

    // Save value
    r->path = results[0];


    // Print debug info and return
    debug(console::cout << "found basic type\n";)
    return std::dynamic_basic_cast<__base_ST>(r);
}