#include "FunctionSignature.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>

std::string cmp::ST_FunctionSignature::getCategoryName(bool plural) const {
    return plural ? "function signatures" : "function signature";
}

std::string cmp::Pattern_Elm_FunctionSignature::genDecoratedValue(bool article) const {
    return article
        ? "a Function Signature"
        :   "Function Signature"
    ;
}

ulong cmp::Pattern_Elm_FunctionSignature::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_FunctionSignature::init() {
    using enum ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::Type(),
        tk::Identifier(),
        tk::Keyword(KEYWORD_ROUND_L),
        op::Optional((ulong)-1,
            op::OneOf(
                tk::Identifier(), //BUG implement routine parameters
                tk::Identifier()  //BUG implement parameter packs
            ),
            op::Optional((ulong)-1, op::Loop(1UL,
                tk::Keyword(KEYWORD_COMMA),
                op::OneOf(
                    tk::Identifier(), //BUG implement routine parameters
                    tk::Identifier()  //BUG implement parameter packs
                )
            ))
        ),
        tk::Keyword(KEYWORD_ROUND_R)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_FunctionSignature::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_FunctionSignature>();

    r->name = results[0]->asIdentifier();

    // Print debug info and return
    debug(console::cout << "Found function signature " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}