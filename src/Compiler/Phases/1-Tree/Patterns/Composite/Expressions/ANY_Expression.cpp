#include "ANY_Expression.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::Pattern_ANY_Expression::genDecoratedValue(bool article) const {
    return article
        ? "an expression"
        :   "expression"
    ;
}

ulong cmp::Pattern_ANY_Expression::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_ANY_Expression::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            re::Expr_Literal(),
            re::Expr_TryExpression(),
            re::Expr_TemporaryVariable(),
            re::Expr_FunctionCall(),

            //! These are indistinguishable until the declarations are fully parsed
            //!     - Routine parameters
            //!     - Variables
            //!     - Function values
            //!     - Temporary variables
            re::Expr_SimpleReferenceExpression()

            //FIXME implement operator calls
            // //! One of the identifiers of an operator call
            // //! The parameters aren't determined until the declaration is found and fully parsed
            // re::Expr_OperatorCallPart()
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_ANY_Expression::generateData(std::vector<ptr<__base_ST>> const &results) const {
    debug(console::cout << "found expression\n";)
    return std::dynamic_pointer_cast<__base_ST>(results[0]);
}