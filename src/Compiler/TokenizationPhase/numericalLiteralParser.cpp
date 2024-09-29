#include <sstream>
#include <cmath>
#include "numericalLiteralParser.hpp"
#include "Utils/errors.hpp"





/**
 * @brief Parses the long or double literal that strarts at index <index> and ends before the first character t hat's not in its pattern.
 * @param b The buffer that contains the numerical literal.
 * @param index The index at which the numerical literal starts.
 * @param rawLiteralLen The raw length of the literal (the number of characters it occupies in the original source code)
 * @return The string value of the literal token, or nullptr if one was not found.
 */
cmp::TokenValue* cmp::parseNumericalLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen) {
    std::stringstream r;
    std::optional<char> const &c0 = b->str[index];
    if(!c0.has_value()) {
        *rawLiteralLen = 0;
        return nullptr;
    }
    if(!std::isdigit(*c0)) {
        cmp::TokenValue* text = parseTextNumericalLiteral(b, index, rawLiteralLen);
        return text;
    }




    // Find the base of the literal and set the starting index
    uint base = 0;
    std::string pattern;
    std::string baseName;
    ulong i;
    std::optional<char> const &c1 = b->str[index + 1];
    if(c0 == '0' && c1.has_value() && std::isalpha(*c1)) {
        i = index + 2;
        switch(*c1) {
            case 'b': { base = 2;  pattern = "01";                     baseName = "binary";      break; }
            case 'o': { base = 2;  pattern = "01234567";               baseName = "octal";       break; }
            case 'd': { base = 10; pattern = "0123456789";             baseName = "decimal";     break; }
            case 'x': { base = 16; pattern = "0123456789abcdefABCDEF"; baseName = "hexadecimal"; break; }
            default: {
                utils::printError(
                    ERROR_CMP_LITERAL_BASE_INVALID,
                    utils::ErrType::COMPILER,
                    ElmCoords(b, index, index + 1),
                    std::string("Unknown numerical base prefix \"0") + *c1 + "\".\n" +
                    "Valid prefixes are \"0b\", \"0o\", \"0d\", \"0x\"."
                );
            }
        }
    }
    else {
        i = index;
        base = 10;
        pattern = "0123456789";
        baseName = "decimal";
    }




    // Convert the string to a token value
    bool isFloat = false;
    while(true) {
        std::optional<char> const &c = b->str[i];
        if(c.has_value()) {
            if(pattern.find(*c)) {
                r << *c;
                ++i;
            }
            else if(c == '_') {
                ++i;
            }
            else if(c == '.') {
                if(isFloat) {
                    utils::printError(
                        ERROR_CMP_LITERAL_DIGITS_MISSING,
                        utils::ErrType::COMPILER,
                        ElmCoords(b, index, i - 1),
                        ElmCoords(b, i, i),
                        "Duplicate radix point in " + baseName + " literal.\n"
                        "Only one is allowed."
                    );
                }
                isFloat = true;
                ++i;
            }
            else if(std::isalnum(*c)) {
                utils::printError(
                    ERROR_CMP_LITERAL_DIGITS_INVALID,
                    utils::ErrType::COMPILER,
                    ElmCoords(b, index, i),
                    ElmCoords(b, i, i),
                    std::string("Invalid digit \"") + *c + "\" in " + baseName + " literal.\n"
                );
            }
            else break;
        }
        else break;
    }
    if(r.tellp() == 0) {
        utils::printError(
            ERROR_CMP_LITERAL_DIGITS_MISSING,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, index + 1),
            ElmCoords(b, index + 2, index + 2),
            "Missing digits in " + baseName + " literal.\n"
            "At least 1 digit is required."
        );
    }




    // Return the value
    if(isFloat) {
        return new TokenValue_DBL(strToDbl(r.str(), base));
    }
    else {
        return new TokenValue_LNG(std::stoul(r.str(), nullptr, base));
    }
}







/**
 * @brief Parses literals with value "inf", "nan", "true" or "false"
 * @param b The buffer that contains the numerical literal.
 * @param index The index at which the numerical literal starts.
 * @param rawLiteralLen The raw length of the literal (the number of characters it occupies in the original source code)
 * @return The string value of the literal token, or nullptr if one was not found.
 */
cmp::TokenValue* cmp::parseTextNumericalLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen) {
    //FIXME
}








/**
 * @brief Converts a string to a double value.
 * @param s The string.
 * @param base The numerical base of the value.
 * @return The value as a double.
 */
double cmp::strToDbl(std::string const &s, uint base) {
    double r = 0;

    // Calculate integer part
    ulong i;
    for(i = 0; s[i] != '.'; ++i) {
        char digit = s[i];
        double value = digit - (std::isdigit(digit) ? '0' : (std::isupper(digit) ? 'A' : 'a') - 10);
        r *= base;
        r += value;
    }


    // Calculate fractional part
    for(int j = i + 1; j < s.length(); ++j) {
        char digit = s[i];
        double value = digit - (std::isdigit(digit) ? '0' : (std::isupper(digit) ? 'A' : 'a') - 10);
        double fraction = pow(10, j - i); //! 10 for first digit, 100 for second etc...
        r += value / fraction;
    }


    // Return the calculated value
    return r;





    // // Find radix point index
    // size_t point;
    // if(s[s.length() - 1] == '.') {
    //     s += '0';
    //     point = s.length() - 1;
    // }
    // else {
    //     point = s.find('.');
    // }


    // // Calculate integer part
    // ulong _int = std::stoul(s.substr(0, point), nullptr, base);
    // double _frc = 0.0;


    // // Calculate fractional part manually
    // if(point != std::string::npos) {
    //     std::string _frcStr = s.substr(point + 1);
    //     for (size_t i = 0; i < _frcStr.size(); ++i) {
    //         if (_frcStr[i] == '1') {
    //             _frc += 1.0 / (1 << (i + 1));
    //         }
    //     }
    // }

    // return _int + b;
}