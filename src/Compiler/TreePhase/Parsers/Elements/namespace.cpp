#include "namespace.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/Parsers/Generic/scope.hpp"




cmp::ST_Namespace* cmp::parseNamespace(TokenizedSource* b, ulong index) {
    ST_Namespace* r = new ST_Namespace();
    ulong i = index;




    // Parse name
    std::optional<Token> const &t0 = (*b)[i];
    if(!t0.has_value()){
        utils::printError(
            ERROR_CMP_NAMESPACE_NAME_MISSING,
            utils::ErrType::COMPILER,
            ElmCoords(b, index - 1, i - 1),
            ElmCoords(b, i - 1,     i - 1),
            "Incomplete definition of Namespace.\n"
            "The identifier is missing."
        );
    }
    else if(!t0->isIdentifier()){
        utils::printError(
            ERROR_CMP_NAMESPACE_NAME_MISSING,
            utils::ErrType::COMPILER,
            ElmCoords(b, index - 1, i),
            ElmCoords(b, i,         i),
            "Expected an identifier, but the " + t0->genDecoratedValue() + " was found instead."
        );
    }
    r->name = t0->getValue_Identifier();
    ++i;




    // Parse contents and check if they are allowed
    ulong scopeLen;
    std::vector<__base_ST*> const &children = generic_parseScope(b, i, true, &scopeLen);
    i += scopeLen;

    for(ulong j = 0; j < children.size(); ++j) {
        if(!r->isChildAllowed(children[i])) {
            utils::printError(
                ERROR_CMP_SCOPE_CHILD_NOT_ALLOWED,
                utils::ErrType::COMPILER,
                ElmCoords(b, index - 1,             children[i]->tokenEnd),
                ElmCoords(b, children[i]->tokenBgn, children[i]->tokenEnd),
                children[i]->getCategoryName(true) + " are not allowed within namespaces"
            );
        }
        r->addChild(children[i]);
    }




    // Set the element length and return the node
    r->tokenBgn = index - 1; //! Account for the namespace keyword
    r->tokenEnd = i - 1;
    return r;
}