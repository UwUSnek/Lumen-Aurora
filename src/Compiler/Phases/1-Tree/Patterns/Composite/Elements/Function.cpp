#include "Function.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>
#include <optional>

std::string cmp::ST_Function::getCategoryName(bool plural) const {
    return plural ? "function declarations" : "function declaration";
}

std::string cmp::Pattern_Elm_Function::genDecoratedValue(bool article) const {
    return article
        ? "a Function declaration"
        :   "Function declaration"
    ;
}

ulong   cmp::Pattern_Elm_Function::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Elm_Function::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::FunctionSignature(),
        op::Loop((ulong)-1,
            op::Optional(1UL, re::Constraint()),
            re:RoutineBody()
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_Function::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Function>();

    // Save signature
    r->signature = results[0]->asFunctionSignature();


    // Save bodies
    for(ulong i = 1; i < results.size(); ++i) {
        std::optional<ptr<ST_Constraint>> _constraint;
        if(results[i].isConstraint()) {
            _constraint = results[i].asConstriant();
            ++i;
        }
        else {
            _constraint = std::nullopt;
        }
        r->bodies.emplace_back(_constraint, results[i].asRoutineSignature());
    }


    // Print debug info and return
    debug(console::cout << "found function " << r->signature->name->s << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}