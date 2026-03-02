#include "UnnamedScope.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Sttm_UnnamedScope::getCategoryName(bool plural) const {
    return plural ? "unnamed scopes" : "unnamed scope";
}

std::string cmp::Pattern_Sttm_UnnamedScope::genDecoratedValue(bool article) const {
    return article
        ? "a Unnamed Scope"
        :   "Unnamed Scope"
    ;
}

ulong cmp::Pattern_Sttm_UnnamedScope::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Sttm_UnnamedScope::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_UnnamedScope::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_UnnamedScope>();

    // Save data

    // Print debug info and return
    debug(console::cout << "found unnamed scope\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}