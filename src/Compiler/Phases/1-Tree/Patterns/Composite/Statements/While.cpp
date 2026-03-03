#include "While.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_While::getCategoryName(bool plural) const {
    return plural ? "while statements" : "while statement";
}

std::string cmp::Pattern_Sttm_While::genDecoratedValue(bool article) const {
    return article
        ? "a While statement"
        :   "While statement"
    ;
}

ulong cmp::Pattern_Sttm_While::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_While::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::Sttm_WhileHeader(),
        re::ANY_Statement()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_While::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_While>();

    // Save data
    r->header    = results[0];
    r->statement = results[1];

    // Print debug info and return
    debug(console::cout << "found while statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}