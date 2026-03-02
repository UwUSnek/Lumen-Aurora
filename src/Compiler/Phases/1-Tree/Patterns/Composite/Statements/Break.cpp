#include "Break.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Break::getCategoryName(bool plural) const {
    return plural ? "break statements" : "break statement";
}

std::string cmp::Pattern_Sttm_Break::genDecoratedValue(bool article) const {
    return article
        ? "a Break statement"
        :   "Break statement"
    ;
}

ulong cmp::Pattern_Sttm_Break::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Break::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Break::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Break>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found break statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}