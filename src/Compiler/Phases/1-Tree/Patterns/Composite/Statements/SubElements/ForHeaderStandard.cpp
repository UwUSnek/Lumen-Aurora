#include "ForHeaderStandard.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include "Utils/ptr.hpp"
#include <memory>


std::string cmp::ST_Sttm_ForHeaderStandard::getCategoryName(bool plural) const {
    return plural ? "standard for headers" : "standard for header";
}

std::string cmp::Pattern_Sttm_ForHeaderStandard::genDecoratedValue(bool article) const {
    return article
        ? "a Standard for header"
        :   "Standard for header"
    ;
}

ulong cmp::Pattern_Sttm_ForHeaderStandard::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Sttm_ForHeaderStandard::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::Optional(1UL, op::OneOf(
            re::ANY_Expression(),
            re::Elm_Variable()
        )),
        tk::Keyword(KEYWORD_SEMICOLON),
        op::Optional(1UL, re::ANY_Expression()),
        tk::Keyword(KEYWORD_SEMICOLON),
        op::Optional(1UL, re::ANY_Expression())
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Sttm_ForHeaderStandard::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Sttm_ForHeaderStandard>();
    ulong i = 0;



    // Save data
    {
        // Initializer expression
        if(results[i]->isKeyword()) { r->initializer = nullptr;    i++;    }
        else {                        r->initializer = results[i]; i += 2; }

        // Condition expression
        if(results[i]->isKeyword()) { r->condition = nullptr;    i++;    }
        else {                        r->condition = results[i]; i += 2; }

        // Iteration expression
        if(results.size() == i) r->iteration = nullptr;
        else                    r->iteration = results[i];
    }



    // Print debug info and return
    debug(console::cout << "found standard for header\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}