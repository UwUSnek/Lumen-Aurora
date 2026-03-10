#include "Try.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Expr_Try::getCategoryName(bool plural) const {
    return plural ? "try expressions" : "try expression";
}

std::string cmp::Pattern_Expr_Try::genDecoratedValue(bool article) const {
    return article
        ? "a Try expression"
        :   "Try expression"
    ;
}

ulong cmp::Pattern_Expr_Try::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Expr_Try::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        //BUG
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Expr_Try::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Expr_Try>();

    // Save value
    r->moduleContents = results[2].asModule();


    // Print debug info and return
    debug(console::cout << "found try expression\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}