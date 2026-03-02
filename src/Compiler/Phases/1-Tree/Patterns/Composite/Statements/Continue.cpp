#include "Continue.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Continue::getCategoryName(bool plural) const {
    return plural ? "continue statements" : "continue statement";
}

std::string cmp::Pattern_Sttm_Continue::genDecoratedValue(bool article) const {
    return article
        ? "a Continue statement"
        :   "Continue statement"
    ;
}

ulong cmp::Pattern_Sttm_Continue::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Continue::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Continue::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Continue>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found continue statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}