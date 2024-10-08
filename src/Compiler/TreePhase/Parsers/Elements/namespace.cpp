#include "namespace.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/Parsers/Generic/scope.hpp"




cmp::ST_Namespace* cmp::parseNamespace(TokenizedSource* b, ulong index) {
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_NAMESPACE)) {
        return nullptr;
    }
    ST_Namespace* r = new ST_Namespace();
    ulong i = index + 1;




    // Parse name
    std::optional<Token> const &t1 = (*b)[i];
    if(!t1.has_value()){
        utils::printError(
            ERROR_CMP_NAMESPACE_NAME_MISSING, utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            ElmCoords(b, i - 1, i - 1),
            "Incomplete definition of Namespace.\n"
            "The identifier is missing."
        );
    }
    else if(!t1->isIdentifier()){
        utils::printError(
            ERROR_CMP_NAMESPACE_NAME_MISSING, utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Incomplete definition of Namespace.\n"
            "Expected an identifier, but the " + t1->genDecoratedValue() + " was found instead."
        );
    }
    r->name = t1->getValue_Identifier();
    ++i;




    // Parse contents and check if they are allowed
    ulong scopeLen;
    std::vector<__base_ST*> const &children = generic_parseScope(b, i, true, &scopeLen);
    i += scopeLen;

    for(ulong j = 0; j < children.size(); ++j) {
        if(!r->isChildAllowed(children[j])) {
            utils::printError(
                ERROR_CMP_SCOPE_CHILD_NOT_ALLOWED, utils::ErrType::COMPILER,
                ElmCoords(b, index,                 children[j]->tokenEnd),
                ElmCoords(b, children[j]->tokenBgn, children[j]->tokenEnd),
                children[j]->getCategoryName(true) + " are not allowed within namespaces"
            );
        }
        r->addChild(children[j]);
    }




    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}