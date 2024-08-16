//!                                                           ICF                                                           !//
//!                                                 Intermediate Code Format                                                !//


#pragma once
#include <string>
#include <vector>
#include <regex>
#include "Utils/utils.hpp"




namespace pre {
    enum class IFC_CleanType {
        NONE,
        STRING,
        CHAR,
        OTHER
    };
    std::string IFC_CleanTypeToString(IFC_CleanType e);




    /**
     * @brief Represents a chunk of code created by the cleanup phase.
     *      s:          The clean string value.
     *      t:          The type of the value.
     *      OG_lineNum: The line number of the raw value in the original file.
     *      OG_height:  The number of NLCs the raw value contained.
     *      OG_start:   The index of the start of the raw value in the original file (first character).
     *      OG_end:     The index of the end of the raw value in the original file (last character + 1).
     */
    class ICF_CleanElm {
    public:
        IFC_CleanType t;
        std::string s;
        ulong OG_lineNum;  //! Redundant, used for performance reasons
        ulong OG_height;   //! Redundant, used for performance reasons
        ulong OG_start;
        ulong OG_end;


        ICF_CleanElm(IFC_CleanType _t, std::string _s, ulong _OG_lineNum, ulong _OG_height, ulong _OG_start, ulong _OG_end) :
            t(_t),
            s(_s),
            OG_lineNum(_OG_lineNum),
            OG_height(_OG_height),
            OG_start(_OG_start),
            OG_end(_OG_end) {
        }
    };




    /**
     * @brief Represents the source code after the cleanup phase.
     *      Each element contains an contiguous chunk of code (meaning that it didn't contain any whitespace in the original file).
     *      LCTs do not split the code.
     */
    class ICF_Clean {
    public:
        std::vector<ICF_CleanElm> elms;

        constexpr ICF_CleanElm& operator[](std::size_t i) noexcept {
            return elms[i];
        }

        std::string toString(){
            static ulong elmTypeNameMaxSpace = 16;
            std::string r = "";
            for(ICF_CleanElm &elm : elms) {
                std::string elmTypeName = IFC_CleanTypeToString(elm.t);
                r += elmTypeName;
                r.append(std::max(1l, (long)elmTypeNameMaxSpace - (long)elmTypeName.length()), ' ');
                r += "| ";
                r += std::regex_replace(std::regex_replace(elm.s, std::regex("\n"), ansi::black + "\\n" + ansi::reset), std::regex(" "), ansi::black + "·" + ansi::reset);
                r += "\n";
            }
            return r;
        }
    };
}