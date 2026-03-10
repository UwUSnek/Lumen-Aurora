#include "ReferenceExpression.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Expr_ReferenceExpression::getCategoryName(bool plural) const {
    return plural ? "reference expressions" : "reference expression";
}

std::string cmp::Pattern_Expr_ReferenceExpression::genDecoratedValue(bool article) const {
    return article
        ? "a Reference expression"
        :   "Reference expression"
    ;
}

ulong cmp::Pattern_Expr_ReferenceExpression::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Expr_ReferenceExpression::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::ANY_Path()
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Expr_ReferenceExpression::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Expr_ReferenceExpression>();

    // Save value
    r->path = results[0];


    // Print debug info and return
    debug(console::cout << "found simplere ference expression\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}