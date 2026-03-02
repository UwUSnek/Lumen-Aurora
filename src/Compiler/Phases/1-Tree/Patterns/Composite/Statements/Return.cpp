#include "Return.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Return::getCategoryName(bool plural) const {
    return plural ? "return statements" : "return statement";
}

std::string cmp::Pattern_Sttm_Return::genDecoratedValue(bool article) const {
    return article
        ? "a Return statement"
        :   "Return statement"
    ;
}

ulong cmp::Pattern_Sttm_Return::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Return::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Return::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Return>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found return statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}