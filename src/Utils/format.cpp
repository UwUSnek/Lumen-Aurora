#include "format.hpp"
#include "Utils/ansi.hpp"
#include <cmath>
#include <format>




/**
* Helper function to add comma separators to a number string.
*/
std::string addCommas(unsigned long amount) {
    std::string result = std::to_string(amount);
    int insert_position = static_cast<int>(result.length()) - 3;

    while (insert_position > 0) {
        result.insert(insert_position, ",");
        insert_position -= 3;
    }

    return result;
}




/**
* Returns the value <amount> expressed as a string and formatted using thousands separators.
* @param amount The amount to format.
* @return The formatted amount.
*/
std::string format::amount(unsigned long amount) {
    return format::amount(amount, false, true);
}




/**
* Returns the value <amount> expressed as a string and formatted as specified.
* @param amount The amount to format.
* @param x Whether the amount should be prefixed with a lowercase "x".
* @param thousandsSeparator Whether to use a separator between thousands.
* @return The formatted amount.
*/
std::string format::amount(unsigned long amount, bool x, bool thousandsSeparator) {
    std::string result;

    if(thousandsSeparator) {
        result = addCommas(amount);
    } else {
        result = std::to_string(amount);
    }

    // Add trailing x if requested
    return x ? result + "x" : result;
}




/**
 * @brief Returns a string containing the character.
 *      Invisible characters are replaced with visible glyphs.
 * @param c The character.
 * @param col The column number of the character (its index relative to the start of the line). This is used to render tab characters.
 * @param useColor Whether to color invisible characters black. Default: false.
 * @return The formatted character.
 */
std::string format::whitespace(char c, ulong col, bool useColor) {
    /**/ if(c ==  ' ') return useColor ? ansi::bright_black + "·" + ansi::reset : "·";
    else if(c == '\n') return useColor ? ansi::bright_black + "⏷" + ansi::reset : "⏷";
    else if(c == '\t') {
        const char* s[] = { "╶──╴", "╶─╴", "╶╴", "-" };
        std::string tabRender = s[col % 4];
        return useColor ? ansi::bright_black + tabRender + ansi::reset : tabRender;
    }
    else return std::string(1, c);
}








/**
 * @brief Converts the value <n> to a string that contains the 3 most significant characters
 *      and 1 letter indicating the multiplier, up to quadrillions.
 *      e.g. shortenInteger(4859495221) returns "4.8b"
 * @param n The number to convert.
 * @return The string representation of <n>.
 */
std::string format::shortenInteger(unsigned long n) {
    static constexpr unsigned long k = 1000;
    static constexpr unsigned long m = 1000000;
    static constexpr unsigned long b = 1000000000;
    static constexpr unsigned long t = 1000000000000;
    static constexpr unsigned long q = 1000000000000000;

    double value;
    std::string suffix;

    if(n >= q) {
        value = static_cast<double>(n) / q;
        suffix = "q";
    }
    else if(n >= t) {
        value = static_cast<double>(n) / t;
        suffix = "t";
    }
    else if(n >= b) {
        value = static_cast<double>(n) / b;
        suffix = "b";
    }
    else if(n >= m) {
        value = static_cast<double>(n) / m;
        suffix = "m";
    }
    else if(n >= k) {
        value = static_cast<double>(n) / k;
        suffix = "k";
    }
    else {
        // Less than 1000
        return std::to_string(n);
    }

    // Format with 1 decimal place
    std::string result = std::format("{:.1f}", value);

    // Truncate to 3 characters max (e.g., "999" or "99.9" or "9.9")
    if(result.length() > 3) {
        result = result.substr(0, 3);
    }

    // Remove trailing period if present
    if(!result.empty() && result.back() == '.') {
        result.pop_back();
    }

    return result + suffix;
}








/**
 * @brief Creates a string with the format <minutes>:<seconds>.<milliseconds> using <n> as the total number of milliseconds.
 *      If the duration is more than 59:59.999 (3599999ms), the format changes to <hours>hrs, expressing the number of hours using 1 decimal digit.
 *      If the duration is less than 0ms, the output strings will contain "    0.000"
 *      Unnecessary leading zeroes and separators are replaced by space characters.
 * @param milliseconds The number of milliseconds.
 * @param unit If true, appends the appropriate unit suffix (ms/s/hrs) to the output.
 * @return The string representation of <n>.
 *      The maximum length of the returned string is 9 characters (12 with unit).
 */

std::string format::milliseconds(long _ms, bool unit) {
    if(_ms < 0) _ms = 0;

    // Hours
    if(_ms > 3599999) {
        float hrs = (float)_ms / 3600000.0f;
        return std::format("{:.1f}{}", hrs, unit ? "hrs" : "");
    }

    // Minutes:seconds.milliseconds
    else if(_ms >= 60000) {
        long min = _ms / 60000;
        long sec = _ms / 1000 % 60;
        long ms =  _ms % 1000;
        return std::format("{}:{:02}.{:03}{}", min, sec, ms, unit ? "min" : "");
    }

    // Seconds.milliseconds
    else if(_ms >= 1000) {
        long sec = _ms / 1000;
        long ms = _ms % 1000;
        return std::format("{}.{:03}{}", sec, ms, unit ? "s" : "");
    }

    // Milliseconds
    else {
        return std::format("{}{}", _ms, unit ? "ms" : "");
    }
}







/**
* Helper function to get the size label based on type and unit.
*/
std::string getSizeLabel(format::SizeLabelType type, long value, format::SizeUnits scale) {
    using enum format::SizeUnits;

    // Symbol label
    if(type == format::SizeLabelType::SYMBOL) {
        switch (scale) {
            case b:  return "b";
            case B:  return "B";
            case KB: return "KB"; case KiB: return "KiB";
            case MB: return "MB"; case MiB: return "MiB";
            case GB: return "GB"; case GiB: return "GiB";
            case TB: return "TB"; case TiB: return "TiB";
            case PB: return "PB"; case PiB: return "PiB";
            case EB: return "EB"; case EiB: return "EiB";
            case ZB: return "ZB"; case ZiB: return "ZiB";
            case YB: return "YB"; case YiB: return "YiB";
        }
    }

    // Full label
    else {
        std::string label;
        switch (scale) {
            case b:  label = "Bit";       break;
            case B:  label = "Byte";      break;
            case KB: label = "Kilobyte";  break; case KiB: label = "Kibibyte"; break;
            case MB: label = "Megabyte";  break; case MiB: label = "Mebibyte"; break;
            case GB: label = "Gigabyte";  break; case GiB: label = "Gibibyte"; break;
            case TB: label = "Terabyte";  break; case TiB: label = "Tebibyte"; break;
            case PB: label = "Petabyte";  break; case PiB: label = "Pebibyte"; break;
            case EB: label = "Exabyte";   break; case EiB: label = "Exbibyte"; break;
            case ZB: label = "Zettabyte"; break; case ZiB: label = "Zebibyte"; break;
            case YB: label = "Yottabyte"; break; case YiB: label = "Yobibyte"; break;
        }
        if (value != 1) {
            label += "s";
        }
        return label;
    }


    return "";
}




/**
 * Helper function to check if a unit is binary (uses 1024 base).
 */
bool isBinaryUnit(format::SizeUnits unit) {
    using enum format::SizeUnits;
    return
        unit == KiB || unit == MiB ||
        unit == GiB || unit == TiB ||
        unit == PiB || unit == EiB ||
        unit == ZiB || unit == YiB
    ;
}




/**
 * Helper function to remove trailing zeros from a decimal number string.
 */
std::string trimTrailingZeros(std::string str) {
    if (str.find('.') != std::string::npos) {
        // Remove trailing zeros after decimal point
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        // Remove trailing decimal point if no decimals left
        if (str.back() == '.') {
            str.pop_back();
        }
    }
    return str;
}




/**
 * Returns the value <bytes> expressed as a string in the specified unit.
 * @param bytes The number of bytes to format. This MUST be >= 0.
 * @param unit The size unit to display the value in.
 * @return The formatted byte size.
 */
std::string format::formatSize(unsigned long bytes, SizeUnits unit) {
    return formatSize(bytes, unit, SizeLabelType::SYMBOL, 2);
}




/**
 * Returns the value <bytes> expressed as a string in the specified unit.
 * @param bytes The number of bytes to format. This MUST be >= 0.
 * @param unit The size unit to display the value in.
 * @param labelType The label type (SYMBOL or FULL).
 * @return The formatted byte size.
 */
std::string format::formatSize(unsigned long bytes, SizeUnits unit, SizeLabelType labelType) {
    return formatSize(bytes, unit, labelType, 2);
}




/**
 * Returns the value <bytes> expressed as a string in the specified unit.
 * @param bytes The number of bytes to format. This MUST be >= 0.
 * @param unit The size unit to display the value in.
 * @param labelType The label type (SYMBOL or FULL).
 * @param precision The maximum number of decimal places to show. This is redundant if displaying in Bits.
 * @return The formatted byte size.
 */
std::string format::formatSize(unsigned long bytes, SizeUnits unit, SizeLabelType labelType, int precision) {

    // Custom logic for bits
    if(unit == SizeUnits::b) {
        unsigned long bits = bytes * 8UL;
        std::string label = getSizeLabel(labelType, static_cast<long>(bits), unit);
        return std::format("{} {}", bits, label);
    }

    // Determine if using binary (1024) or decimal (1000) units
    bool isBinary = isBinaryUnit(unit);
    unsigned long divisor = isBinary ? 1024UL : 1000UL;

    // Get the exponent for the requested unit
    int exp;
    switch(unit) {
        using enum SizeUnits;
        case b:  case B:   exp = 0; break;
        case KB: case KiB: exp = 1; break;
        case MB: case MiB: exp = 2; break;
        case GB: case GiB: exp = 3; break;
        case TB: case TiB: exp = 4; break;
        case PB: case PiB: exp = 5; break;
        case EB: case EiB: exp = 6; break;
        case ZB: case ZiB: exp = 7; break;
        case YB: case YiB: exp = 8; break;
    }

    // Calculate the value in the requested unit
    double value = static_cast<double>(bytes) / std::pow(divisor, exp);

    // Format with the specified precision
    std::string formatted = std::format("{:.{}f}", value, precision);
    formatted = trimTrailingZeros(formatted);

    // Get the appropriate label
    std::string label = getSizeLabel(labelType, static_cast<long>(value), unit);

    // Return the formatted size with label
    return formatted + " " + label;
}