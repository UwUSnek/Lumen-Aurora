#include "Main/ALC.hpp"
#include "tokenizationPhase.hpp"
#include "Main/FatalErrorException.hpp"
#include "Main/errors.hpp"

#include "Misc/whitespaceCounter.hpp"
#include "Parsers/keywordsIdentifiers.hpp"
#include "Parsers/textLiterals.hpp"
#include "Parsers/numericalLiterals.hpp"








void cmp::__internal_startTokenizationPhase(ptr<pre::AnnotatedSource<false>> b, ptr<TokenizedSource<false>> r) {

    ulong i = 0;
    while((*b)[i]) {


        // Detect whitespace and split tokens if any is found
        if(ulong wsLen = misc::countWhitespace(*b, i)) {
            increaseLocalProgress(wsLen);
            i += wsLen;
            continue;
        }




        // Parse numerical literlas
        ulong lenOutput;
        auto tokenValue = parseNumericalLiteral(b, i, &lenOutput);
        if(tokenValue) {
            increaseMaxProgress(1, PhaseID::C1_TreeCreation);
            increaseLocalProgress(lenOutput);
            *r += Token(b->substr(i, lenOutput), tokenValue, (*b)[i]->meta, (*b)[i + lenOutput - 1]->meta);
            i += lenOutput;
            continue;
        }




        // Parse out alphanumeric and symbolic tokens (keywords, meta keywords and identifiers)
        auto               token = parseSymbolicKeyword   (b, i);
        if( token.empty()) token = parseSymbolicIdentifier(b, i);
        if( token.empty()) token = parseAlphanumericToken (b, i);
        if(!token.empty()) {
            ptr<TokenValue> _tokenValue;

            // If the token is a known keyword
            if(auto keywordType = reservedTokensMap.find(token); keywordType != reservedTokensMap.end()) {
                switch(keywordType->second) {
                    using enum cmp::ReservedTokenId;
                    case TMP_LITERAL_TRUE:  { _tokenValue = newptr<TK_Bool   >(true);  break; }
                    case TMP_LITERAL_FALSE: { _tokenValue = newptr<TK_Bool   >(false); break; }
                    case TMP_LITERAL_INF:   { _tokenValue = newptr<TK_Double >(std::numeric_limits<double>::infinity());  break; }
                    case TMP_LITERAL_NAN:   { _tokenValue = newptr<TK_Double >(std::numeric_limits<double>::quiet_NaN()); break; }
                    default:                { _tokenValue = newptr<TK_Keyword>(keywordType->second); }
                }
            }

            // If not, treat it as an identifier
            else {
                _tokenValue = newptr<TK_Identifier>(token);
            }

            // Push token to output array and update buffer index
            increaseMaxProgress(1, PhaseID::C1_TreeCreation);
            increaseLocalProgress(token.length());
            *r += Token(token, _tokenValue, (*b)[i]->meta, (*b)[i + token.length() - 1]->meta);
            // *r += Token(b->substr(i, token.length()), _tokenValue, (*b)[i]->meta, (*b)[i + token.length() - 1]->meta);
            i += token.length();
            continue;
        }




        // Parse text literals
        /**/            tokenValue = parseTextLiteral(b, i, &lenOutput, TextLiteralType::STRING);
        if(!tokenValue) tokenValue = parseTextLiteral(b, i, &lenOutput, TextLiteralType::CHAR);
        if(tokenValue) {
            increaseMaxProgress(1, PhaseID::C1_TreeCreation);
            increaseLocalProgress(lenOutput);
            *r += Token(b->substr(i, lenOutput), tokenValue, (*b)[i]->meta, (*b)[i + lenOutput - 1]->meta);
            i += lenOutput;
            continue;
        }




        // Print an error if none of the parsers could detect anything (current character is invalid)
        utils::printError(
            ErrorCode::ERROR_CMP_CHARACTER_INVALID,
            utils::ErrType::COMPILER,
            ElmCoords(b, i, i),
            std::string("Invalid character '") + (*b)[i]->c + "'.\n" +
            "This character is not allowed within Lumen or Aurora source code.",
            true //TODO recovery system. skip to the first token that makes sense
        );
        //FIXME ^ decode multi byte characters to print them in the error
    }
}








void cmp::startTokenizationPhase(ptr<pre::AnnotatedSource<false>> b, ptr<TokenizedSource<false>> r) {

    // Try to execute the subphase
    try {
        __internal_startTokenizationPhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipe and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        // std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[(int)PhaseID::C0_Tokenization].totalProgress->setProgressColor(ansi::red); //FIXME change progress bar color on failure
    }
}