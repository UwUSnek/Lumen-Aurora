#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "BasicPath.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Path_Basic::getCategoryName(bool plural) const {
    return plural ? "basic paths" : "basic path";
}

std::string cmp::Pattern_Path_Basic::genDecoratedValue(bool article) const {
    return article
        ? "a Basic path"
        :   "Basic path"
    ;
}

ulong cmp::Pattern_Path_Basic::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_Path_Basic::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        tk::Identifier(),
        op::Optional((ulong)-1,
            tk::Keyword(KEYWORD_DOT),
            tk::Identifier()
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Path_Basic::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Path_Basic>();

    // Save value
    for(ulong i = 0; i < results.size(); i += 2) {
        r->paths.emplace_back(results[i]->asIdentifier());
    }


    // Print debug info and return
    debug(console::cout << "found basic path\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}