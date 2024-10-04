#include "ALC.hpp"
#include "tokenizationPhase.hpp"
#include "Utils/errors.hpp"

#include "whitespaceCounter.hpp"
#include "parseKeywordsIdentifiers.hpp"
#include "parseTextLiterals.hpp"
#include "parseNumericalLiterals.hpp"








void cmp::startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r) {

    ulong i = 0;
    while(b->str[i].has_value()) {


        // Detect whitespace and split tokens if any is found
        ulong wsLen = countWhitespace(b, i);
        if(wsLen) {
            i += wsLen;
            increaseLocalProgress(wsLen); //FIXME
            continue;
        }




        // Parse numerical literlas
        ulong lenOutput;
        TokenValue *tokenValue = parseNumericalLiteral(b, i, &lenOutput);
        if(tokenValue) {
            *r += Token(tokenValue, *b->meta[i], *b->meta[i + lenOutput]);
            i += lenOutput;
            increaseLocalProgress(lenOutput); //FIXME
            continue;
        }




        // Parse out alphanumeric and symbolic tokens (keywords, meta keywords and identifiers)
        std::optional<std::string> token = parseAlphanumericToken(b, i);
        if(!token.has_value())     token = parseSymbolicToken(b, i);
        if(token.has_value()) {
            TokenValue *tokenValue;

            // If the token is a known keyword
            auto keywordType = reservedTokensMap.find(*token);
            if(keywordType != reservedTokensMap.end()) {
                switch(keywordType->second) {
                    case ReservedTokenId::TMP_LITERAL_TRUE:  { tokenValue = new TK_Bool   (true);  break; }
                    case ReservedTokenId::TMP_LITERAL_FALSE: { tokenValue = new TK_Bool   (false); break; }
                    case ReservedTokenId::TMP_LITERAL_INF:   { tokenValue = new TK_Double (std::numeric_limits<double>::infinity());  break; }
                    case ReservedTokenId::TMP_LITERAL_NAN:   { tokenValue = new TK_Double (std::numeric_limits<double>::quiet_NaN()); break; }
                    default:                                 { tokenValue = new TK_Keyword(keywordType->second); }
                }
            }

            // If not, treat it as an identifier
            else {
                tokenValue = new TK_String(*token);
            }

            // Push token to output array and update buffer index
            *r += Token(tokenValue, *b->meta[i], *b->meta[i + token->length()]);
            i += token->length();
            increaseLocalProgress(token->length()); //FIXME
            continue;
        }




        // Parse text literals
        tokenValue = parseStrLiteral(b, i, &lenOutput);
        if(tokenValue) {
            *r += Token(tokenValue, *b->meta[i], *b->meta[i + lenOutput]);
            i += lenOutput;
            increaseLocalProgress(lenOutput); //FIXME
            continue;
        }
        tokenValue = parseCharLiteral(b, i, &lenOutput);
        if(tokenValue) {
            *r += Token(tokenValue, *b->meta[i], *b->meta[i + lenOutput]);
            i += lenOutput;
            increaseLocalProgress(lenOutput); //FIXME
            continue;
        }




        // Print an error if none of the parsers could detect anything (current character is invalid)
        utils::printError(
            ERROR_CMP_CHARACTER_INVALID,
            utils::ErrType::COMPILER,
            ElmCoords(b, i, i),
            std::string("Invalid character '") + *b->str[i] + "'.\n" +
            "This character is not allowed within Lumen or Aurora source code."
        );
        //FIXME ^ decode multi byte characters to print them in the error
    }


    r->closePipe();
}
