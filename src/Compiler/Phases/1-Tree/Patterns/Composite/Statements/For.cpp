#include "For.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_For::getCategoryName(bool plural) const {
    return plural ? "for statements" : "for statement";
}

std::string cmp::Pattern_Sttm_For::genDecoratedValue(bool article) const {
    return article
        ? "a For statement"
        :   "For statement"
    ;
}

ulong cmp::Pattern_Sttm_For::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_For::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            re::Sttm_ForHeaderStandard(),
            re::Sttm_ForHeaderRange(),
            re::Sttm_ForHeaderForEach()
        ),
        re::ANY_Statement(),
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_For::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_For>();

    // Save data
    r->header    = results[0];
    r->statement = results[1];

    // Print debug info and return
    debug(console::cout << "found for statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}