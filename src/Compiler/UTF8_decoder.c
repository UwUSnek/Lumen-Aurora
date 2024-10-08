// The MIT License (MIT)

// Copyright (c) 2021 Pierre Pharel Angoro Abiaga

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.




/*
    Modified by UwU_Snek
    Original code @ https://github.com/PierrePharel/utf8_decoder
*/




#include <stddef.h>

#include "UTF8_decoder.h"








#ifndef UTF8_DECODER_LOGGER_H
#define UTF8_DECODER_LOGGER_H

#define INFO_COLOR ("\x1b[32m")
#define WARNING_COLOR ("\x1b[31m")
#define RESET_COLOR ("\x1b[0m")

#define INFO 0
#define WARNING 1

#include <stdio.h>

#ifdef UTF8_DECODER_LOG
    #define Log(type, ...)\
    {\
        if (type == INFO)\
            printf("%s%s: ", INFO_COLOR, __func__);\
        else\
            printf("%s%s: ", WARNING_COLOR, __func__);\
        printf(__VA_ARGS__);\
        printf("\n");\
        printf(RESET_COLOR);\
    }
#else
    #define Log(type, ...)
#endif

#endif








Utf8Type utf8type(const utf8chr_t* hex_str, int32_t* cdp)
{
    int32_t codepoint = 0;
    short shift = 0;

    for (const utf8chr_t* s = hex_str; *s != END; ++ s)
    {
        codepoint = ((codepoint << shift) | hexchr_to_hex(*s));
        shift = 4;
    }

    if (cdp != NULL)
        *cdp = codepoint;

    if (codepoint >= 0x0000 && codepoint <= 0x007f)
        return US_ASCII;
    else if (codepoint > 0x007f && codepoint <= 0x07ff)
        return LatinExtra;
    else if (codepoint > 0x07ff && codepoint <= 0xffff)
        return BasicMultiLingual;
    else if (codepoint > 0xffff && codepoint <= 0x10ffff)
        return OthersPlanesUnicode;

    return OutRange;
}

utf8chr_t hexchr_to_hex(const utf8chr_t hex_chr)
{
    if ('0' <= hex_chr && hex_chr <= '9')
        return hex_chr - 48;
    if ('A' <= hex_chr && hex_chr <= 'F')
        return hex_chr - 55;
    if ('a' <= hex_chr && hex_chr <= 'f')
        return hex_chr - 87;
    return -1;
}

void utf8decode(const utf8chr_t* hex_str, utf8chr_t* dest)
{
    int32_t cdp = 0;
    Utf8Type type = utf8type(hex_str, &cdp);
    // those are common to all the utf8 types
    utf8chr_t c1 = hexchr_to_hex(hex_str[1]);
    utf8chr_t c2 = hexchr_to_hex(hex_str[2]);
    utf8chr_t c3 = hexchr_to_hex(hex_str[3]);

    switch (type)
    {
        case US_ASCII:
        {
            int32_t c0 = 0;
            utf8type(hex_str, &c0);
            dest[0] = c0;
            dest[1] = END;

            Log(INFO, "%X", dest[0]);
            break;
        }

        case LatinExtra:
        {
            // first char
            dest[0] = LATIN_EXTRA_BEGIN;
            dest[0] |= ((c1 & 0x7) << 2);
            dest[0] |= ((c2 & 0xc) >> 2);

            // second char
            dest[1] = SECONDARY_CHAR_BEGIN;
            dest[1] |= ((c2 & 0x3) << 4);
            dest[1] |= c3;

            // end char
            dest[2] = END;

            Log(INFO, "%X %X", dest[0], dest[1]);
            break;
        }

        case BasicMultiLingual:
        {
            utf8chr_t c0 = hexchr_to_hex(hex_str[0]);

            // first char
            dest[0] = BASIC_MULTILINGUAL_BEGIN;
            dest[0] |= c0;

            // second char
            dest[1] = SECONDARY_CHAR_BEGIN;
            dest[1] |= (c1 << 2);
            dest[1] |= ((c2 & 0xc) >> 2);

            // third char
            dest[2] = SECONDARY_CHAR_BEGIN;
            dest[2] |= ((c2 & 0x3) << 4);
            dest[2] |= c3;

            // end char
            dest[3] = END;

            Log(INFO, "%X %X %X", dest[0], dest[1], dest[2]);
            break;
        }

        case OthersPlanesUnicode:
        {
            utf8chr_t c0 = hexchr_to_hex(hex_str[0]);
            utf8chr_t c4 = hexchr_to_hex(hex_str[4]);

            if (cdp <= 0xfffff)
            {
                // first char
                dest[0] = OTHERS_PLANES_UNICODE_BEGIN;
                dest[0] |= ((c0 & 0xc) >> 2);

                // second char
                dest[1] = SECONDARY_CHAR_BEGIN;
                dest[1] |= ((c0 & 0x3) << 4);
                dest[1] |= c1;

                // third char
                dest[2] = SECONDARY_CHAR_BEGIN;
                dest[2] |= (c2 << 2);
                dest[2] |= ((c3 & 0xc) >> 2);

                // fourth char
                dest[3] = SECONDARY_CHAR_BEGIN;
                dest[3] |= ((c3 & 0x3) << 4);
                dest[3] |= c4;

                // end char
                dest[4] = END;

                Log(INFO, "%X %X %X %X", dest[0], dest[1], dest[2],  dest[3]);
            }
            else
            {
                utf8chr_t c5 = hexchr_to_hex(hex_str[5]);

                // first char
                dest[0] = OTHERS_PLANES_UNICODE_BEGIN;
                dest[0] |= ((c0 & 0x1) << 2);
                dest[0] |= ((c1 & 0xc) >> 2);

                // second char
                dest[1] = SECONDARY_CHAR_BEGIN;
                dest[1] |= ((c1 & 0x3) << 4);
                dest[1] |= ((c1 & 0xc) >> 2);
                dest[1] |= c2;

                // third char
                dest[2] = SECONDARY_CHAR_BEGIN;
                dest[2] |= (c3 << 2);
                dest[2] |= ((c4 & 0xc) >> 2);

                // fourth char
                dest[3] = SECONDARY_CHAR_BEGIN;
                dest[3] |= ((c4 & 0x3) << 4);
                dest[3] |= c5;

                // end char
                dest[4] = END;

                Log(INFO, "%X %X %X %X", dest[0], dest[1], dest[2],  dest[3]);
            }

            break;
        }

        case OutRange:
            *dest = END;
#if defined (UTF8_DECODER_LOG)
            Log(WARNING, "String is empty, we are out of utf8 range !");
#endif
            break;
    }


}

bool utf8valid(const utf8chr_t *str)
{
    const utf8chr_t* s = str;

    if (str == NULL)
    {
        Log(WARNING, "Null string");
        return UTF8_BAD_CHAR;
    }

    while (*s != END)
    {
        if (0xf0 == (0xf8 & *s))
        {
            // ensure each of the 3 following bytes in this 4 byte
            // utf8 codepoint began with 0b10xxxxxx
            if ((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])) || (0x80 != (0xc0 & s[3])))
                return UTF8_BAD_CHAR;

            // ensure that our utf8 codepoint ended after 4 byte
            if (0x80 == (0xc0 & s[4]))
                return UTF8_BAD_CHAR;

            // ensure that the top 5 bits of this 4 byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if ((0 == (0x07 & s[0])) && (0 == (0x30 & s[1])))
                return UTF8_BAD_CHAR;

            // 4-byte utf8 code point (began with 0b11110xxx)
            s += 4;
        }
        else if (0xe0 == (0xf0 & *s))
        {
            // ensure each of the 2 following bytes in this 3 byte
            // utf8 codepoint began with 0b10xxxxxx
            if ((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])))
                return UTF8_BAD_CHAR;

            // ensure that our utf8 codepoint ended after 3 bytes
            if (0x80 == (0xc0 & s[3]))
                return UTF8_BAD_CHAR;

            // ensure that the top 5 bits of this 3 byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if((0 == (0x0f & s[0])) && (0 == (0x20 & s[1])))
                return UTF8_BAD_CHAR;

            // 3-byte utf8 code point (began with 0b1110xxxx)
            s += 3;
        }
        else if (0xc0 == (0xe0 & *s))
        {
            // ensure the 1 following byte in this 2 byte
            // utf8 codepoint began with 0b10xxxxxx
            if (0x80 != (0xc0 & s[1]))
                return UTF8_BAD_CHAR;

            // ensure that our utf8 codepoint ended after 2 bytes
            if (0x80 == (0xc0 & s[2]))
                return UTF8_BAD_CHAR;

            // ensure that the top 4 bits of this 2 byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if (0 == (0x1e & s[0]))
                return UTF8_BAD_CHAR;

            // 2-byte utf8 code point (began with 0b110xxxxx)
            s += 2;
        }
        else if (0x00 == (0x80 & *s))
        {
            // 1-byte ascii (began with 0b0xxxxxxx)
            s += 1;
        }
        else
        {
            // we have an invalid 0b1xxxxxxx utf8 code point entry
            return UTF8_BAD_CHAR;
        }
    }

    return UTF8_GOOD_CHAR;
}

int32_t utf8codepoint(const char* str)
{
    int32_t codepoint = 0;
    const utf8chr_t* s = (const utf8chr_t*)str;

    if (utf8valid((const utf8chr_t*)str))
    {
        if (str == NULL)
        {
            Log(WARNING, "Null string");
            return -1;
        }

        while (*s != END)
        {
            if (0xf0 == (0xf8 & *s))
            {
                // four byte
                codepoint = ((0x07 & s[0]) << 18) | ((0x3f & s[1]) << 12) | ((0x3f & s[2]) << 6) | (0x3f & s[3]);
                s += 4;
            }
            else if (0xe0 == (0xf0 & *s))
            {
                // three byte
                codepoint = ((0x0f & s[0]) << 12) | ((0x3f & s[1]) << 6) | (0x3f & s[2]);
                s += 3;
            }
            else if (0xc0 == (0xe0 & *s))
            {
                // two byte
                codepoint = ((0x1f & s[0]) << 6) | (0x3f & s[1]);
                s += 2;
            }
            else if (0x00 == (0x80 & *s))
            {
                // one byte
                codepoint = s[0];
                ++ s;
            }
            else
            {
                Log(WARNING, "Invalid codepoint");
                return -1;
            }
        }
    }

    return codepoint;
}

void utf8chr(const int32_t codepoint, utf8chr_t* dest)
{
    if (codepoint >= 0x0000 && codepoint <= 0x007f)
    {
        dest[0] = codepoint;
        dest[1] = END;
        Log(INFO, "%X", dest[0]);
    }
    else if (codepoint > 0x007f && codepoint <= 0x07ff)
    {
        dest[0] = LATIN_EXTRA_BEGIN;
        dest[1] = SECONDARY_CHAR_BEGIN;

        if (codepoint > 0xff)
            dest[0] |= (codepoint >> 6);
        dest[0] |= ((codepoint & 0xc0) >> 6);
        dest[1] |= (codepoint & 0x3f);

        dest[2] = END;
        Log(INFO, "%X %X", dest[0], dest[1]);
    }
    else if (codepoint > 0x07ff && codepoint <= 0xffff)
    {
        dest[0] = BASIC_MULTILINGUAL_BEGIN;
        dest[1] = SECONDARY_CHAR_BEGIN;
        dest[2] = SECONDARY_CHAR_BEGIN;

        if (codepoint > 0xfff)
            dest[0] |= ((codepoint & 0xf000) >> 12);
        dest[1] |= ((codepoint & 0xf00) >> 6);
        dest[1] |= ((codepoint & 0xf0) >> 6);
        dest[2] |= (codepoint & 0x30);
        dest[2] |= (codepoint & 0xf);

        dest[3] = END;
        Log(INFO, "%X %X %X", dest[0], dest[1], dest[2]);
    }
    else if (codepoint > 0xffff && codepoint <= 0x10ffff)
    {
        dest[0] = OTHERS_PLANES_UNICODE_BEGIN;
        dest[1] = SECONDARY_CHAR_BEGIN;
        dest[2] = SECONDARY_CHAR_BEGIN;
        dest[3] = SECONDARY_CHAR_BEGIN;

        if (codepoint > 0xfffff)
            dest[0] |= ((codepoint & 0x100000) >> 18);
        dest[0] |= ((codepoint & 0xc0000) >> 18);
        dest[1] |= ((codepoint & 0x30000) >> 12);
        dest[1] |= ((codepoint & 0xf000) >> 12);
        dest[2] |= ((codepoint & 0xf00) >> 6);
        dest[2] |= ((codepoint & 0xc0) >> 6);
        dest[3] |= (codepoint & 0x30);
        dest[3] |= (codepoint & 0xf);

        dest[4] = END;
        Log(INFO, "%X %X %X %X", dest[0], dest[1], dest[2], dest[3]);
    }
    else
    {
        *dest = END;
        Log(WARNING, "String is empty, we are out of utf8 range !");
    }
}

#undef LATIN_EXTRA_BEGIN
#undef BASIC_MULTILINGUAL_BEGIN
#undef OTHERS_PLANES_UNICODE_BEGIN
#undef SECONDARY_CHAR_BEGIN
#undef END
#undef UTF8_BAD_CHAR
#undef UTF8_GOOD_CHAR