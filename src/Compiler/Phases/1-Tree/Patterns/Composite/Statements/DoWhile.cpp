#include "DoWhile.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_DoWhile::getCategoryName(bool plural) const {
    return plural ? "do-while statements" : "do-while statement";
}

std::string cmp::Pattern_Sttm_DoWhile::genDecoratedValue(bool article) const {
    return article
        ? "a Do-while statement"
        :   "Do-while statement"
    ;
}

ulong cmp::Pattern_Sttm_DoWhile::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_DoWhile::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_DoWhile::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_DoWhile>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found do-while statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}