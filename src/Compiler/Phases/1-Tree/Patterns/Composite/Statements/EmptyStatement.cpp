#include "EmptyStatement.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_EmptyStatement::getCategoryName(bool plural) const {
    return plural ? "empty statements" : "empty statement";
}

std::string cmp::Pattern_Sttm_EmptyStatement::genDecoratedValue(bool article) const {
    return article
        ? "a Empty Statement"
        :   "Empty Statement"
    ;
}

ulong cmp::Pattern_Sttm_EmptyStatement::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_EmptyStatement::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_EmptyStatement::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_EmptyStatement>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found empty statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}