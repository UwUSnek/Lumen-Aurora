#include "If.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_If::getCategoryName(bool plural) const {
    return plural ? "if statements" : "if statement";
}

std::string cmp::Pattern_Sttm_If::genDecoratedValue(bool article) const {
    return article
        ? "a If statement"
        :   "If statement"
    ;
}

ulong cmp::Pattern_Sttm_If::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_If::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_If::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_If>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found if statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}