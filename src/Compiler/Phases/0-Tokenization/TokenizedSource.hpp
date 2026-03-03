#pragma once
#include <map>
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"



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
    X(KEYWORD_ADDR)             /* addr */                                                                         \
    X(KEYWORD_DEREF)            /* @ */                                                                            \
    \
    X(KEYWORD_IF)               /* if */                                                                           \
    X(KEYWORD_MATCH)            /* match */                                                                        \
    X(KEYWORD_FOR)              /* for */                                                                          \
    X(KEYWORD_WHILE)            /* while */                                                                        \
    X(KEYWORD_DO)               /* do */                                                                           \
    X(KEYWORD_CONTINUE)         /* continue */                                                                     \
    X(KEYWORD_BREAK)            /* break */                                                                        \
    X(KEYWORD_ONCE)             /* once */                                                                         \
    X(KEYWORD_ELSE)             /* else */                                                                         \
    X(KEYWORD_THEN)             /* then */                                                                         \
    X(KEYWORD_RETURN)           /* return */                                                                       \
    \
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
    X(KEYWORD_REFLECTION)       /* : */                                                                            \
    \
    \
    X(META_KEYWORD_TEMPLATE)    /* template */                     /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_LT)          /* < */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_GT)          /* > */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_TYPE)        /* type */                         /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_TARGET)      /* ! */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_ATS)         /* ? */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_WHEN)        /* when */                         /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_PRIORITY)    /* priority */                     /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_AS)          /* as */                           /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_IN)          /* in */                           /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_TO)          /* to */                           /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_ASSIGN)      /* = */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_COLON)       /* : */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_PTR)         /* * */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_AND)         /* & */                            /*FIXME check these in identifier patterns */   \
    X(META_KEYWORD_ELLIPSIS)    /* ... */                          /*FIXME check these in identifier patterns */   \
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
    extern std::map<std::string, ReservedTokenId, std::less<>> reservedTokensMap;








    struct TokenValue {
        virtual std::string getCategoryName() const { return ""; };

        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~TokenValue() = default;
    };


    // String literal
    struct TK_Str : TokenValue {
        std::string v;
        explicit TK_Str(const std::string&_v) : v(_v) {}
        std::string getCategoryName() const override;
    };

    // Char literal
    struct TK_Char : TokenValue {
        char v;
        explicit TK_Char(char _v) : v(_v) {}
        std::string getCategoryName() const override;
    };

    // Ulong literal
    struct TK_Ulong : TokenValue {
        ulong v;
        explicit TK_Ulong(ulong _v) : v(_v) {}
        std::string getCategoryName() const override;
    };

    // Double literal
    struct TK_Double : TokenValue {
        double v;
        explicit TK_Double(double _v) : v(_v) {}
        std::string getCategoryName() const override;
    };

    // Boolean literal
    struct TK_Bool : TokenValue {
        bool v;
        explicit TK_Bool(bool _v) : v(_v) {}
        std::string getCategoryName() const override;
    };

    // Identifier
    struct TK_Identifier : TokenValue {
        std::string v;
        explicit TK_Identifier(const std::string &_v) : v(_v) {}
        std::string getCategoryName() const override;
    };

    // Keyword
    struct TK_Keyword : TokenValue {
        ReservedTokenId v;
        explicit TK_Keyword(ReservedTokenId _v) : v(_v) {}
        std::string getCategoryName() const override;
    };








    struct Token {
        std::string OG_Value;
        ptr<TokenValue> value;               // The value of the token (number or string)
        pre::AnnotatedSourceMeta start;     // The index, line, columns and file of the first character relative to the original source code
        pre::AnnotatedSourceMeta end;       // The index, line, columns and file of the last  character relative to the original source code


        Token() = delete;
        Token(const Token &token) = default;
        Token(const std::string& _OG_Value, ptr<TokenValue> _value, pre::AnnotatedSourceMeta const &_start, pre::AnnotatedSourceMeta const &_end) :
            OG_Value(_OG_Value),
            value(_value),
            start(_start),
            end(_end) {
        }
        std::string genDecoratedValue() const {
            return isStr() || isChar()
                ? value->getCategoryName() + " \"" + OG_Value.substr(1, OG_Value.length() - 2) + "\""
                : value->getCategoryName() + " \"" + OG_Value + "\""
            ;
        }




        const std::string &getValue_Str       () const;
        char               getValue_Char      () const;
        ulong              getValue_Ulong     () const;
        double             getValue_Double    () const;
        bool               getValue_Bool      () const;
        const std::string &getValue_Identifier() const;
        ReservedTokenId    getValue_Keyword   () const;

        bool isStr       () const;
        bool isChar      () const;
        bool isUlong     () const;
        bool isDouble    () const;
        bool isBool      () const;
        bool isIdentifier() const;
        bool isKeyword   () const;
        bool isKeyword   (ReservedTokenId id) const;
    };


    template<bool safeRealloc=true>
    using TokenizedSource = VectorPipe<Token, safeRealloc>;
}