#include "ALC.hpp"
#include "tokenizationPhase.hpp"
#include "Utils/errors.hpp"

#include "whitespaceCounter.hpp"
#include "keywordIdentifierParser.hpp"
#include "textLiteralParser.hpp"
#include "numericalLiteralParser.hpp"








void cmp::startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r) {

    // Copy total progress from preprocessing phase
    phaseDataArrayLock.lock();
    ulong max = phaseDataArray[PhaseID::Preprocessing].totalProgress->max.load(); //! Max doesn't change after merging the files
    phaseDataArrayLock.unlock();
    increaseMaxProgress(max);
    //TODO subtract deleted characters from the preprocessor max
    //FIXME include and macro stuff doesnt get counted. fix this
    //FIXME subtract macro calls and definitions
    //FIXME or do this a different way




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
            auto keywordType = keywordMap.find(*token);
            if(keywordType != keywordMap.end()) {
                // tokenType = keywordType->second;
                tokenValue = new TokenValue_KEY(keywordType->second);
            }

            // If not, treat it as an identifier
            else {
                // tokenType = TokenType::IDENTIFIER;
                tokenValue = new TokenValue_STR(*token);
            }

            // Push token to output array and update buffer index
            // *r += Token(*token, tokenType, *b->meta[i], *b->meta[i + token->length()]);
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
}
