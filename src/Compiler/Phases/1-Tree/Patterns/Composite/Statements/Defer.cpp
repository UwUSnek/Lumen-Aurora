#include "Defer.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Defer::getCategoryName(bool plural) const {
    return plural ? "defer statements" : "defer statement";
}

std::string cmp::Pattern_Sttm_Defer::genDecoratedValue(bool article) const {
    return article
        ? "a Defer statement"
        :   "Defer statement"
    ;
}

ulong cmp::Pattern_Sttm_Defer::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Defer::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Defer::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Defer>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found defer statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}