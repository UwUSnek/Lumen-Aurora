#include "Match.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Match::getCategoryName(bool plural) const {
    return plural ? "match statements" : "match statement";
}

std::string cmp::Pattern_Sttm_Match::genDecoratedValue(bool article) const {
    return article
        ? "a Match statement"
        :   "Match statement"
    ;
}

ulong cmp::Pattern_Sttm_Match::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Match::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Match::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Match>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found match statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}