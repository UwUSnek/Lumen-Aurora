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
    X(KEYWORD_DOT)              /* . */                                                                            \
    X(KEYWORD_REFLECTION)       /* :: //TODO maybe use :? tho it might conflict with the metakeyword*/             \
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
    enum class ReservedTokenId : ulong {
        #define X(e) e,
        LIST_RESERVED_TOKENS
        #undef X
    };




    struct TokenValue {
        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~TokenValue(){}
    };
    struct TK_String     : TokenValue { std::string     v; TK_String    (std::string const &_v) : v(_v) {}};      // String literal
    struct TK_Char       : TokenValue { char            v; TK_Char      (char               _v) : v(_v) {}};      // Char literal
    struct TK_Long       : TokenValue { ulong           v; TK_Long      (ulong              _v) : v(_v) {}};      // Ulong literal
    struct TK_Double     : TokenValue { double          v; TK_Double    (double             _v) : v(_v) {}};      // Double literal
    struct TK_Bool       : TokenValue { bool            v; TK_Bool      (bool               _v) : v(_v) {}};      // Boolean literal
    struct TK_Identifier : TokenValue { std::string     v; TK_Identifier(std::string const &_v) : v(_v) {}};      // Identifier
    struct TK_Keyword    : TokenValue { ReservedTokenId v; TK_Keyword   (ReservedTokenId    _v) : v(_v) {}};      // Keyword





    extern std::map<std::string, ReservedTokenId> reservedTokensMap;




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




        std::string           &getValue_String    ()       { return ((TK_String    *)value)->v; }
        char                  &getValue_Char      ()       { return ((TK_Char      *)value)->v; }
        ulong                 &getValue_Long      ()       { return ((TK_Long      *)value)->v; }
        double                &getValue_Double    ()       { return ((TK_Double    *)value)->v; }
        bool                  &getValue_Bool      ()       { return ((TK_Bool      *)value)->v; }
        std::string           &getValue_Identifier()       { return ((TK_Identifier*)value)->v; }
        ReservedTokenId       &getValue_Keyword   ()       { return ((TK_Keyword   *)value)->v; }

        std::string     const &getValue_String    () const { return ((TK_String    *)value)->v; }
        char            const &getValue_Char      () const { return ((TK_Char      *)value)->v; }
        ulong           const &getValue_Long      () const { return ((TK_Long      *)value)->v; }
        double          const &getValue_Double    () const { return ((TK_Double    *)value)->v; }
        bool            const &getValue_Bool      () const { return ((TK_Bool      *)value)->v; }
        std::string     const &getValue_Identifier() const { return ((TK_Identifier*)value)->v; }
        ReservedTokenId const &getValue_Keyword   () const { return ((TK_Keyword   *)value)->v; }

        bool isString    () const { return dynamic_cast<TK_String    *>(value); }
        bool isChar      () const { return dynamic_cast<TK_Char      *>(value); }
        bool isLong      () const { return dynamic_cast<TK_Long      *>(value); }
        bool isDouble    () const { return dynamic_cast<TK_Double    *>(value); }
        bool isBool      () const { return dynamic_cast<TK_Bool      *>(value); }
        bool isIdentifier() const { return dynamic_cast<TK_Identifier*>(value); }
        bool isKeyword   () const { return dynamic_cast<TK_Keyword   *>(value); }
        bool isKeyword   (ReservedTokenId id) const { return dynamic_cast<TK_Keyword*>(value) && getValue_Keyword() == id; }
    };


    typedef VectorPipe<Token> TokenizedSource;
}