#pragma once
#include <map>
#include "Preprocessor/SegmentedCleanSource.hpp"



#define LIST_TOKEN_TYPE                                                                                            \
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
    X(KEYWORD_TRUE)             /* true */                                                                         \
    X(KEYWORD_FALSE)            /* false */                                                                        \
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
    X(KEYWORD_CANDIDATE)        /* Meta keywords (not confirmed, might be identifiers depending on the context) */ \
\
    X(IDENTIFIER)               /* Identifiers */                                                                  \
\
    X(LITERAL_NUM)              /* Number literals */                                                              \
    X(LITERAL_STR)              /* String literals */                                                              \
    X(LITERAL_CHR)              /* Char literals */                                                                \











namespace cmp {
    enum class TokenType : ulong {
        #define X(e) e,
        LIST_TOKEN_TYPE
        #undef X
    };





    extern std::map<std::string, TokenType> keywordMap;
    // std::map<std::string, TokenType> metaKeywordMap;




    struct Token {
        std::string value;              // The trimmed, clean stirng value of the token
        TokenType type;                 // The type of the token
        pre::CleanSourceMeta start;     // The index, line, columns and file of the first character relative to the original source code
        pre::CleanSourceMeta end;       // The index, line, columns and file of the last  character relative to the original source code

        Token(std::string const &_value, TokenType _type, pre::CleanSourceMeta const &_start, pre::CleanSourceMeta const &_end) :
            value(_value),
            type(_type),
            start(_start),
            end(_end) {
        }
    };


    typedef VectorPipe<Token> TokenizedSource;
}