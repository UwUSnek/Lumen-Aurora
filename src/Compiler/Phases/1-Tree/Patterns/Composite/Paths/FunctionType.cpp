#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "FunctionType.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Type_Function::getCategoryName(bool plural) const {
    return plural ? "function types" : "function type";
}

std::string cmp::Pattern_Type_Function::genDecoratedValue(bool article) const {
    return article
        ? "a Function type"
        :   "Function type"
    ;
}

ulong cmp::Pattern_Type_Function::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Type_Function::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::ANY_Type(),
        tk::Keyword(KEYWORD_ROUND_L)
        op::Optional((ulong)-1, op::Loop((ulong)-1,
            re::ANY_Type()
        )),
        tk::Keyword(KEYWORD_ROUND_R)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Type_Function::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Type_Function>();

    // Save value
    r->returnType = results[0];
    for(ulong i = 2; i < results.size() - 1; ++i) {
        r->paramTypes.emplace_back(results[i]);
    }


    // Print debug info and return
    debug(console::cout << "found function type\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}