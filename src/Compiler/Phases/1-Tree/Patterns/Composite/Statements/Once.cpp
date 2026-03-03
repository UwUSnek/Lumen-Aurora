#include "Once.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_Once::getCategoryName(bool plural) const {
    return plural ? "once statements" : "once statement";
}

std::string cmp::Pattern_Sttm_Once::genDecoratedValue(bool article) const {
    return article
        ? "a Once statement"
        :   "Once statement"
    ;
}

ulong cmp::Pattern_Sttm_Once::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_Once::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_ONCE),
        re::ANY_Statement()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_Once::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_Once>();

    // Save data
    r->statement = results[1];

    // Print debug info and return
    debug(console::cout << "found once statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}