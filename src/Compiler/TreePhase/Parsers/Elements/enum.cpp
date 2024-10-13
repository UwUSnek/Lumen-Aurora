#include "enum.hpp"
#include "Utils/errors.hpp"

#include "Compiler/TreePhase/Parsers/SubElements/path.hpp"
#include "namespace.hpp"

std::string cmp::ST_Enum::getCategoryName(bool plural) const {
    return plural ? "enum definition" : "enum definitions";
}




cmp::Pattern_Elm_Enum::Pattern_Elm_Enum() : __base_Pattern_Composite(
    tk::Keyword(ReservedTokenId::KEYWORD_ENUM),
    tk::Identifier(),
    tk::Keyword(ReservedTokenId::META_KEYWORD_BASE),
    tk::Identifier(),
    tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
    op::Loop(op::OneOf(
        re::Enum(),
        re::Namespace()
        //FIXME other possible elements
    )),
    tk::Keyword(ReservedTokenId::KEYWORD_CURLY_R)
){}




cmp::__base_ST* cmp::Pattern_Elm_Enum::generateData(std::vector<__base_ST*> const &results) const {
    ST_Enum* r = new ST_Enum;
    r->name = results[1]->asIdentifier();
    r->baseType = nullptr; //FIXME save actual type
    debug(consoleLock.lock(); cout << "found enum " << r->name->s << "\n"; consoleLock.unlock();)
    //TODO contents
    return r;
}