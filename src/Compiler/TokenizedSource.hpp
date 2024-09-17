#pragma once
#include <map>
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace cmp {
    enum class TokenType : ulong {
        KEYWORD_NAMESPACE,        // namespace
        KEYWORD_ENUM,             // enum
        KEYWORD_STRUCT,           // struct
        KEYWORD_CONST,            // const
        KEYWORD_AUTO,             // auto
        KEYWORD_TYPEOF,           // typeof
        KEYWORD_ALIAS,            // alias
        KEYWORD_FROM,             // from
        KEYWORD_IMPORT,           // import
        KEYWORD_EXPORT,           // export
        KEYWORD_TRUE,             // true
        KEYWORD_FALSE,            // false
        KEYWORD_PTR,              // @ //TODO check if this is correct
        KEYWORD_ADDR,             // $

        KEYWORD_IF,               // if
        KEYWORD_ELSE,             // else
        KEYWORD_FOR,              // for
        KEYWORD_WHILE,            // while
        KEYWORD_DO,               // do
        KEYWORD_CONTINUE,         // continue
        KEYWORD_BREAK,            // break
        KEYWORD_ONCE,             // once
        KEYWORD_THEN,             // then
        KEYWORD_RETURN,           // return
        KEYWORD_DELAY,            // delay //TODO prob not used
        KEYWORD_ASYNC,            // async //TODO prob not used //FIXME add to documentation

        KEYWORD_ROUND_L,          // (
        KEYWORD_ROUND_R,          // )
        KEYWORD_CURLY_L,          // {
        KEYWORD_CURLY_R,          // }

        KEYWORD_COMMA,            // ,
        KEYWORD_SEMICOLON,        // ;
        KEYWORD_MEMBER,           // .
        KEYWORD_REFLECTION,       // ::


        META_KEYWORD_TEMPLATE,      // template
        META_KEYWORD_LT,            // <
        META_KEYWORD_GT,            // >
        META_KEYWORD_TYPE,          // type
        META_KEYWORD_TARGET,        // !
        META_KEYWORD_ATS,           // ?
        META_KEYWORD_WHEN,          // when
        META_KEYWORD_PRIORITY,      // priority
        META_KEYWORD_AS,            // as
        META_KEYWORD_TO,            // to
        META_KEYWORD_ASSIGN,        // =
        META_KEYWORD_BASE,          // :
        // META_KEYWORD_ //TODO :
        // META_KEYWORD_ //TODO &




        KEYWORD_CANDIDATE,        // Meta keywords (not confirmed, might be identifiers depending on the context)

        IDENTIFIER,               // Identifiers

        LITERAL_NUM,              // Number literals
        LITERAL_STR,              // String literals
        LITERAL_CHR,              // Char literals
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