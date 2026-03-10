#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "TemporaryVariablePath.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Path_TemporaryVariable::getCategoryName(bool plural) const {
    return plural ? "temporary variable paths" : "temporary variable path";
}

std::string cmp::Pattern_Path_TemporaryVariable::genDecoratedValue(bool article) const {
    return article
        ? "a Temporary Variable path"
        :   "Temporary Variable path"
    ;
}

ulong cmp::Pattern_Path_TemporaryVariable::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Path_TemporaryVariable::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_AT),
        tk::UlongLiteral(),
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Path_TemporaryVariable::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Path_TemporaryVariable>();

    // Save value
    r->index = results[1]->asUlongLiteral()->value;


    // Print debug info and return
    debug(console::cout << "found temporary variable path\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}