#include "ALC.hpp"
#include "tokenizationPhase.hpp"
#include "Utils/ansi.hpp"
#include "Utils/errors.hpp"

#include "whitespaceCounter.hpp"
#include "Parsers/keywordsIdentifiers.hpp"
#include "Parsers/textLiterals.hpp"
#include "Parsers/numericalLiterals.hpp"








void cmp::__internal_startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r) {

    ulong i = 0;
    while(b->str[i].has_value()) {


        // Detect whitespace and split tokens if any is found
        if(ulong wsLen = countWhitespace(b, i)) {
            increaseLocalProgress(wsLen);
            i += wsLen;
            continue;
        }




        // Parse numerical literlas
        ulong lenOutput;
        TokenValue *tokenValue = parseNumericalLiteral(b, i, &lenOutput);
        if(tokenValue) {
            increaseMaxProgress(Compiler_TreeCreation, 1);
            increaseLocalProgress(lenOutput);
            *r += Token(b->str.substr(i, lenOutput), tokenValue, *b->meta[i], *b->meta[i + lenOutput - 1]);
            i += lenOutput;
            continue;
        }




        // Parse out alphanumeric and symbolic tokens (keywords, meta keywords and identifiers)
        std::optional<std::string> token = parseAlphanumericToken(b, i);
        if(!token.has_value())     token = parseSymbolicToken(b, i);
        if(token.has_value()) {
            TokenValue *_tokenValue;

            // If the token is a known keyword
            if(auto keywordType = reservedTokensMap.find(*token); keywordType != reservedTokensMap.end()) {
                switch(keywordType->second) {
                    using enum cmp::ReservedTokenId;
                    case TMP_LITERAL_TRUE:  { _tokenValue = new TK_Bool   (true);  break; }
                    case TMP_LITERAL_FALSE: { _tokenValue = new TK_Bool   (false); break; }
                    case TMP_LITERAL_INF:   { _tokenValue = new TK_Double (std::numeric_limits<double>::infinity());  break; }
                    case TMP_LITERAL_NAN:   { _tokenValue = new TK_Double (std::numeric_limits<double>::quiet_NaN()); break; }
                    default:                { _tokenValue = new TK_Keyword(keywordType->second); }
                }
            }

            // If not, treat it as an identifier
            else {
                _tokenValue = new TK_Identifier(*token);
            }

            // Push token to output array and update buffer index
            increaseMaxProgress(Compiler_TreeCreation, 1);
            increaseLocalProgress(token->length());
            *r += Token(b->str.substr(i, token->length()), _tokenValue, *b->meta[i], *b->meta[i + token->length() - 1]);
            i += token->length();
            continue;
        }




        // Parse delimited literals
        /**/            tokenValue = parseDelimitedLiteral(b, i, &lenOutput, DelimitedLiteralType::STRING);
        if(!tokenValue) tokenValue = parseDelimitedLiteral(b, i, &lenOutput, DelimitedLiteralType::CHAR);
        if(tokenValue) {
            increaseMaxProgress(Compiler_TreeCreation, 1);
            increaseLocalProgress(lenOutput);
            *r += Token(b->str.substr(i, lenOutput), tokenValue, *b->meta[i], *b->meta[i + lenOutput - 1]);
            i += lenOutput;
            continue;
        }




        // Print an error if none of the parsers could detect anything (current character is invalid)
        utils::printError(
            ERROR_CMP_CHARACTER_INVALID,
            utils::ErrType::COMPILER,
            ElmCoords(b, i, i),
            std::string("Invalid character '") + *b->str[i] + "'.\n" +
            "This character is not allowed within Lumen or Aurora source code.",
            true //TODO recovery system. skip to the first token that makes sense
        );
        //FIXME ^ decode multi byte characters to print them in the error
    }
}








void cmp::startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r) {

    // Try to execute the subphase
    try {
        __internal_startTokenizationPhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipe and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        std::scoped_lock lock(phaseDataArrayLock);
        phaseDataArray[Compiler_Tokenization].totalProgress->setProgressColor(ansi::red);
    }
}