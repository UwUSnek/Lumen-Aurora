#pragma once
#include <string>



namespace format {
    enum class SizeLabelType {
        SYMBOL,
        FULL
    };

    enum class SizeUnits {
        b, B,
        KB,  MB,  GB,  TB,  PB,  EB,  ZB,  YB,
        KiB, MiB, GiB, TiB, PiB, EiB, ZiB, YiB
    };

    [[nodiscard]] std::string amount(unsigned long amount);
    [[nodiscard]] std::string amount(unsigned long amount, bool x, bool thousandsSeparator);

    [[nodiscard]] std::string whitespace(char c, ulong col, bool useColor = false);
    [[nodiscard]] std::string shortenInteger(ulong n);
    [[nodiscard]] std::string milliseconds(long n, bool unit = false);

    [[nodiscard]] std::string formatSize(unsigned long bytes, SizeUnits unit);
    [[nodiscard]] std::string formatSize(unsigned long bytes, SizeUnits unit, SizeLabelType labelType);
    [[nodiscard]] std::string formatSize(unsigned long bytes, SizeUnits unit, SizeLabelType labelType, int precision);
}