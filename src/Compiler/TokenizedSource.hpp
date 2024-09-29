#pragma once
#include <map>
#include "Preprocessor/SegmentedCleanSource.hpp"



#define LIST_RESERVED_TOKENS                                                                                       \
    X(KEYWORD_NAMESPACE)        /* namespace */                                                                    \
    X(KEYWORD_ENUM)             /* enum */                                                                         \
    X(KEYWORD_STRUCT)           /* struct */                                                                       \
    X(KEYWORD_CONST)            /* const */                                                                        \
    X(KEYWORD_AUTO)             /* auto */                                                                         \
    X(KEYWORD_TYPEOF)           /* typeof */                                                                       \
    X(KEYWORD_ALIAS)            /* alias */                                                                        \
    X(KEYWORD_FROM)             /* from */                                                                         \
    X(KEYWORD_IMPORT)           /* import */                                                                       \
    X(KEYWORD_EXPORT)           /* export */                                                                       \
    X(KEYWORD_PTR)              /* @ //TODO check if this is correct */                                            \
    X(KEYWORD_ADDR)             /* $ */                                                                            \
\
    X(KEYWORD_IF)               /* if */                                                                           \
    X(KEYWORD_ELSE)             /* else */                                                                         \
    X(KEYWORD_FOR)              /* for */                                                                          \
    X(KEYWORD_WHILE)            /* while */                                                                        \
    X(KEYWORD_DO)               /* do */                                                                           \
    X(KEYWORD_CONTINUE)         /* continue */                                                                     \
    X(KEYWORD_BREAK)            /* break */                                                                        \
    X(KEYWORD_ONCE)             /* once */                                                                         \
    X(KEYWORD_THEN)             /* then */                                                                         \
    X(KEYWORD_RETURN)           /* return */                                                                       \
    X(KEYWORD_DELAY)            /* delay //TODO prob not used */                                                   \
    X(KEYWORD_ASYNC)            /* async //TODO prob not used //FIXME add to documentation */                      \
\
    X(KEYWORD_ROUND_L)          /* ( */                                                                            \
    X(KEYWORD_ROUND_R)          /* ) */                                                                            \
    X(KEYWORD_CURLY_L)          /* { */                                                                            \
    X(KEYWORD_CURLY_R)          /* } */                                                                            \
\
    X(KEYWORD_COMMA)            /* , */                                                                            \
    X(KEYWORD_SEMICOLON)        /* ; */                                                                            \
    X(KEYWORD_MEMBER)           /* . */                                                                            \
    X(KEYWORD_REFLECTION)       /* :: */                                                                           \
\
\
    X(META_KEYWORD_TEMPLATE)    /* template */                                                                     \
    X(META_KEYWORD_LT)          /* < */                                                                            \
    X(META_KEYWORD_GT)          /* > */                                                                            \
    X(META_KEYWORD_TYPE)        /* type */                                                                         \
    X(META_KEYWORD_TARGET)      /* ! */                                                                            \
    X(META_KEYWORD_ATS)         /* ? */                                                                            \
    X(META_KEYWORD_WHEN)        /* when */                                                                         \
    X(META_KEYWORD_PRIORITY)    /* priority */                                                                     \
    X(META_KEYWORD_AS)          /* as */                                                                           \
    X(META_KEYWORD_TO)          /* to */                                                                           \
    X(META_KEYWORD_ASSIGN)      /* = */                                                                            \
    X(META_KEYWORD_BASE)        /* : */                                                                            \
        /* META_KEYWORD_ //TODO : */\
        /* META_KEYWORD_ //TODO & */\
\
\
\
\
    X(TMP_LITERAL_TRUE)         /* Temporary enum value used for text numerical literals */                        \
    X(TMP_LITERAL_FALSE)        /* Temporary enum value used for text numerical literals */                        \
    X(TMP_LITERAL_NAN)          /* Temporary enum value used for text numerical literals */                        \
    X(TMP_LITERAL_INF)          /* Temporary enum value used for text numerical literals */                        \











namespace cmp {
    enum class ReservedTokenId : ulong { //FIXME rename to "KeywordID"
        #define X(e) e,
        LIST_RESERVED_TOKENS
        #undef X
    };



//TODO check if LNG has to be singed or unsigned

    struct TokenValue {};
    struct TokenValue_STR : TokenValue { std::string v; TokenValue_STR(std::string const &_v) : v(_v) {}};      // String literal
    struct TokenValue_CHR : TokenValue { char        v; TokenValue_CHR(char               _v) : v(_v) {}};      // Char literal
    struct TokenValue_LNG : TokenValue { long        v; TokenValue_LNG(long               _v) : v(_v) {}};      // Long literal
    struct TokenValue_DBL : TokenValue { double      v; TokenValue_DBL(double             _v) : v(_v) {}};      // Double literal
    struct TokenValue_BLN : TokenValue { bool        v; TokenValue_BLN(bool               _v) : v(_v) {}};      // Boolean literal
    struct TokenValue_KEY : TokenValue { ReservedTokenId   v; TokenValue_KEY(ReservedTokenId          _v) : v(_v) {}};      // Keyword
    struct TokenValue_ID  : TokenValue { std::string v; TokenValue_ID (std::string const &_v) : v(_v) {}};      // Identifier





    extern std::map<std::string, ReservedTokenId> reservedTokensMap;
    // std::map<std::string, TokenType> metaKeywordMap;




    struct Token {
        TokenValue *value;               // The value of the token (number or string)
        // TokenType type;                 // The type of the token
        pre::CleanSourceMeta start;     // The index, line, columns and file of the first character relative to the original source code
        pre::CleanSourceMeta end;       // The index, line, columns and file of the last  character relative to the original source code

        // Token(std::string const &_value, TokenType _type, pre::CleanSourceMeta const &_start, pre::CleanSourceMeta const &_end) :
        Token(TokenValue *_value, pre::CleanSourceMeta const &_start, pre::CleanSourceMeta const &_end) :
            value(_value),
            // type(_type),
            start(_start),
            end(_end) {
        }


//TODO check if LNG has to be singed or unsign
        // void setValue_STR(std::string const &v) { ((TokenValue_STR*)value)->v = v; }
        // void setValue_CHR(char        const &v) { ((TokenValue_CHR*)value)->v = v; }
        // void setValue_LNG(long        const &v) { ((TokenValue_LNG*)value)->v = v; }
        // void setValue_DBL(double      const &v) { ((TokenValue_DBL*)value)->v = v; }
        // void setValue_KEY(TokenType   const &v) { ((TokenValue_KEY*)value)->v = v; }
        // void setValue_ID (std::string const &v) { ((TokenValue_ID *)value)->v = v; }

        std::string& getValue_STR() { return ((TokenValue_STR*)value)->v; }
        char&        getValue_CHR() { return ((TokenValue_CHR*)value)->v; }
        long&        getValue_LNG() { return ((TokenValue_LNG*)value)->v; }
        double&      getValue_DBL() { return ((TokenValue_DBL*)value)->v; }
        bool&        getValue_BLN() { return ((TokenValue_BLN*)value)->v; }
        ReservedTokenId&   getValue_KEY() { return ((TokenValue_KEY*)value)->v; }
        std::string& getValue_ID () { return ((TokenValue_ID *)value)->v; }
    };


    typedef VectorPipe<Token> TokenizedSource;
}