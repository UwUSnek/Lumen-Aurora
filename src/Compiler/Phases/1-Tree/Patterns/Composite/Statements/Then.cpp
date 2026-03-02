#include "Then.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Then::getCategoryName(bool plural) const {
    return plural ? "then statements" : "then statement";
}

std::string cmp::Pattern_Sttm_Then::genDecoratedValue(bool article) const {
    return article
        ? "a Then statement"
        :   "Then statement"
    ;
}

ulong cmp::Pattern_Sttm_Then::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Then::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Then::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Then>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found then statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}