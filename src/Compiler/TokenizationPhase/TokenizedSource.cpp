#include <sstream>
#include "TokenizedSource.hpp"




std::map<std::string, cmp::ReservedTokenId> cmp::reservedTokensMap  = {
    { "namespace", ReservedTokenId::KEYWORD_NAMESPACE },
    { "enum",      ReservedTokenId::KEYWORD_ENUM      },
    { "struct",    ReservedTokenId::KEYWORD_STRUCT    },
    { "const",     ReservedTokenId::KEYWORD_CONST     },
    { "auto",      ReservedTokenId::KEYWORD_AUTO      },
    { "typeof",    ReservedTokenId::KEYWORD_TYPEOF    },
    { "alias",     ReservedTokenId::KEYWORD_ALIAS     },
    { "from",      ReservedTokenId::KEYWORD_FROM      },
    { "import",    ReservedTokenId::KEYWORD_IMPORT    },
    { "export",    ReservedTokenId::KEYWORD_EXPORT    },

    { "@",         ReservedTokenId::KEYWORD_PTR       },
    { "$",         ReservedTokenId::KEYWORD_ADDR      },
    { "if",        ReservedTokenId::KEYWORD_IF        },
    { "else",      ReservedTokenId::KEYWORD_ELSE      },
    { "for",       ReservedTokenId::KEYWORD_FOR       },
    { "while",     ReservedTokenId::KEYWORD_WHILE     },
    { "do",        ReservedTokenId::KEYWORD_DO        },
    { "continue",  ReservedTokenId::KEYWORD_CONTINUE  },
    { "break",     ReservedTokenId::KEYWORD_BREAK     },
    { "once",      ReservedTokenId::KEYWORD_ONCE      },
    { "then",      ReservedTokenId::KEYWORD_THEN      },
    { "return",    ReservedTokenId::KEYWORD_RETURN    },
    { "delay",     ReservedTokenId::KEYWORD_DELAY     },
    { "async",     ReservedTokenId::KEYWORD_ASYNC     },




    { "(",         ReservedTokenId::KEYWORD_ROUND_L    },
    { ")",         ReservedTokenId::KEYWORD_ROUND_R    },
    { "{",         ReservedTokenId::KEYWORD_CURLY_L    },
    { "}",         ReservedTokenId::KEYWORD_CURLY_R    },

    { ",",         ReservedTokenId::KEYWORD_COMMA      },
    { ";",         ReservedTokenId::KEYWORD_SEMICOLON  },
    { ".",         ReservedTokenId::KEYWORD_DOT        },
    { "::",        ReservedTokenId::KEYWORD_REFLECTION },




    { "template",  ReservedTokenId::META_KEYWORD_TEMPLATE },
    { "<",         ReservedTokenId::META_KEYWORD_LT       },
    { ">",         ReservedTokenId::META_KEYWORD_GT       },
    { "type",      ReservedTokenId::META_KEYWORD_TYPE     },
    { "!",         ReservedTokenId::META_KEYWORD_TARGET   },
    { "?",         ReservedTokenId::META_KEYWORD_ATS      },
    { "when",      ReservedTokenId::META_KEYWORD_WHEN     },
    { "priority",  ReservedTokenId::META_KEYWORD_PRIORITY },
    { "as",        ReservedTokenId::META_KEYWORD_AS       },
    { "to",        ReservedTokenId::META_KEYWORD_TO       },
    { "=",         ReservedTokenId::META_KEYWORD_ASSIGN   },
    { ":",         ReservedTokenId::META_KEYWORD_BASE     },



    { "true",      ReservedTokenId::TMP_LITERAL_TRUE      },
    { "false",     ReservedTokenId::TMP_LITERAL_FALSE     },
    { "nan",       ReservedTokenId::TMP_LITERAL_NAN       },
    { "inf",       ReservedTokenId::TMP_LITERAL_INF       },
};








std::string cmp::TK_String    ::getCategoryName() const { return "string literal"; }
std::string cmp::TK_Char      ::getCategoryName() const { return "char literal";   }
std::string cmp::TK_Long      ::getCategoryName() const { return "ulong literal";  }
std::string cmp::TK_Double    ::getCategoryName() const { return "double literal"; }
std::string cmp::TK_Bool      ::getCategoryName() const { return "bool literal";   }
std::string cmp::TK_Identifier::getCategoryName() const { return "identifier";     }
std::string cmp::TK_Keyword   ::getCategoryName() const { return "keyword";        }


std::string     const &cmp::Token::getValue_String    () const { return ((TK_String    *)value)->v; }
char                   cmp::Token::getValue_Char      () const { return ((TK_Char      *)value)->v; }
ulong                  cmp::Token::getValue_Long      () const { return ((TK_Long      *)value)->v; }
double                 cmp::Token::getValue_Double    () const { return ((TK_Double    *)value)->v; }
bool                   cmp::Token::getValue_Bool      () const { return ((TK_Bool      *)value)->v; }
std::string     const &cmp::Token::getValue_Identifier() const { return ((TK_Identifier*)value)->v; }
cmp::ReservedTokenId   cmp::Token::getValue_Keyword   () const { return ((TK_Keyword   *)value)->v; }

bool cmp::Token::isString    () const { return dynamic_cast<TK_String    *>(value); }
bool cmp::Token::isChar      () const { return dynamic_cast<TK_Char      *>(value); }
bool cmp::Token::isLong      () const { return dynamic_cast<TK_Long      *>(value); }
bool cmp::Token::isDouble    () const { return dynamic_cast<TK_Double    *>(value); }
bool cmp::Token::isBool      () const { return dynamic_cast<TK_Bool      *>(value); }
bool cmp::Token::isIdentifier() const { return dynamic_cast<TK_Identifier*>(value); }
bool cmp::Token::isKeyword   () const { return dynamic_cast<TK_Keyword   *>(value); }
bool cmp::Token::isKeyword   (ReservedTokenId id) const { return dynamic_cast<TK_Keyword*>(value) && getValue_Keyword() == id; }