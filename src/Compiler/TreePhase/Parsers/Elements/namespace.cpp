#include "namespace.hpp"
#include "Utils/errors.hpp"

#include "enum.hpp"

std::string cmp::ST_Namespace::getCategoryName(bool plural) const {
    return plural ? "namespace definition" : "namespace definitions";
}




cmp::Pattern_Elm_Namespace::Pattern_Elm_Namespace() : __base_Pattern_Composite(
    tk::Keyword(ReservedTokenId::KEYWORD_NAMESPACE),
    tk::Identifier(),
    tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
    op::Loop(op::OneOf(
        re::Enum(),
        re::Namespace()
        //FIXME other possible elements
    )),
    tk::Keyword(ReservedTokenId::KEYWORD_CURLY_R)
){}




cmp::__base_ST* cmp::Pattern_Elm_Namespace::generateData(std::vector<__base_ST*> const &results) const {
    ST_Namespace* r = new ST_Namespace;
    r->name = results[1]->asIdentifier();
    debug(consoleLock.lock(); cout << "found namespace " << r->name->s << "\n"; consoleLock.unlock();)
    //TODO contents
    return r;
}