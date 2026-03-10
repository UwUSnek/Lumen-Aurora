#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "AutoType.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Type_Auto::getCategoryName(bool plural) const {
    return plural ? "auto specifiers" : "auto specifier";
}

std::string cmp::Pattern_Type_Auto::genDecoratedValue(bool article) const {
    return article
        ? "an Auto specifier"
        :   "Auto specifier"
    ;
}

ulong cmp::Pattern_Type_Auto::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Type_Auto::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(KEYWORD_AUTO)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Type_Auto::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Type_Auto>();

    //! No data to save

    // Print debug info and return
    debug(console::cout << "found auto specifier\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}