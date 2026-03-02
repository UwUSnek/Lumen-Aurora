#include "Literal.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Expr_Literal::getCategoryName(bool plural) const {
    return plural ? "literal expressions" : "literal expression";
}

std::string cmp::Pattern_Expr_Literal::genDecoratedValue(bool article) const {
    return article
        ? "a Literal expression"
        :   "Literal expression"
    ;
}

ulong cmp::Pattern_Expr_Literal::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Expr_Literal::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            tk::UlongLiteral(),
            tk::DoubleLiteral(),
            tk::BoolLiteral(),
            tk::CharLiteral(),
            tk::StrLiteral()
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Expr_Literal::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Expr_Literal>();

    // Save value
    if(
        results[0]->isUlongLiteral()  ||
        results[0]->isDoubleLiteral() ||
        results[0]->isBoolLiteral()   ||
        results[0]->isCharLiteral()   ||
        results[0]->isStrLiteral()
    ) {
        r->value = results[0];
    }


    // Print debug info and return
    debug(console::cout << "found literal expression\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}