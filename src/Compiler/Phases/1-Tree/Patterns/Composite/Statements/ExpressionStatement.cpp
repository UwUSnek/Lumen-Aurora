#include "ExpressionStatement.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_ExpressionStatement::getCategoryName(bool plural) const {
    return plural ? "expression statements" : "expression statement";
}

std::string cmp::Pattern_Sttm_ExpressionStatement::genDecoratedValue(bool article) const {
    return article
        ? "a Expression Statement"
        :   "Expression Statement"
    ;
}

ulong cmp::Pattern_Sttm_ExpressionStatement::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_ExpressionStatement::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_ExpressionStatement::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_ExpressionStatement>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found expression statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}