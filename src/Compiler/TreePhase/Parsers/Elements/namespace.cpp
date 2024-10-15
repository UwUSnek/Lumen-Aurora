#include "namespace.hpp"
#include "Utils/errors.hpp"

#include "enum.hpp"

std::string cmp::ST_Namespace::getCategoryName(bool plural) const {
    return plural ? "namespace definition" : "namespace definitions";
}




void cmp::Pattern_Elm_Namespace::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(ReservedTokenId::KEYWORD_NAMESPACE),
        tk::Identifier(),
        tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
        op::Optional(op::Loop(op::OneOf(
            re::Enum(),
            re::Namespace()
            //FIXME other possible elements
        ))),
        tk::Keyword(ReservedTokenId::KEYWORD_CURLY_R)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Namespace::generateData(std::vector<__base_ST*> const &results) const {
    ST_Namespace* r = new ST_Namespace;
    r->name = results[1]->asIdentifier();
    debug((cout++ << "found namespace " << r->name->s << "\n")--;)
    //TODO contents
    return dynamic_cast<__base_ST*>(r);
}




std::string cmp::Pattern_Elm_Namespace::genDecoratedValue() const {
    return "Namespace declaration";
}