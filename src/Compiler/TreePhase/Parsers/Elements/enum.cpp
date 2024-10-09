#include "enum.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/Parsers/SubElements/path.hpp"
#include "Compiler/TreePhase/Parsers/Generic/scope.hpp"








cmp::ST_Enum* cmp::parseEnum(TokenizedSource* b, ulong index) {
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_ENUM)) {
        return nullptr;
    }
    ST_Enum* r = new ST_Enum();
    ulong i = index + 1;




    // Parse name
    std::optional<Token> const &t1 = (*b)[i];
    if(!t1.has_value() || !t1->isIdentifier()) {
        utils::printError(
            ERROR_CMP_ENUM_NO_NAME, utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Incomplete definition of Enum.\n" +
            (!t1.has_value() ? "Expected an identifier, but the " + t1->genDecoratedValue() + " was found instead." : "The identifier is missing.")
        );
    }
    r->name = t1->getValue_Identifier();
    ++i;




    // Parse : keyword
    if(!t1.has_value() || !t1->isIdentifier()) {
        utils::printError(
            ERROR_CMP_ENUM_NO_TYPE, utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Incomplete definition of Enum.\n" +
            (!t1.has_value() ? "Expected the keyword \":\", but the " + t1->genDecoratedValue() + " was found instead." : "The \":\" keyword is missing.")
        );
    }
    ++i;




    // Parse base type
    ST_Sub_Path* baseType = parsePath(b, i); //FIXME USE TYPE PARSER INSTEAD
    if(!baseType) {
        utils::printError(
            ERROR_CMP_ENUM_NO_TYPE, utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Incomplete definition of Enum.\n" +
            (!t1.has_value() ? "Expected a type path, but the " + t1->genDecoratedValue() + " was found instead." : "The base type is missing.")
        );
    }
    r->baseType = baseType;
    ++i;




    // Parse elements
    ulong childrenNum;
    std::vector<__base_ST*> const &children = generic_parseScope(b, i, true, &childrenNum);
    //FIXME PARSE OUTPUT


    //FIXME maybe make the parsers functional
    //FIXME printing errors as soon as the syntax is not correct might prevent other parsers from even starting





    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}