#include "Else.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Else::getCategoryName(bool plural) const {
    return plural ? "else statements" : "else statement";
}

std::string cmp::Pattern_Sttm_Else::genDecoratedValue(bool article) const {
    return article
        ? "a Else statement"
        :   "Else statement"
    ;
}

ulong cmp::Pattern_Sttm_Else::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Else::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Else::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Else>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found else statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}