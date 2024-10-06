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








std::string cmp::Token::genDecoratedValue() const {
    std::string a;
    if(isString    ()) a = "string literal";
    if(isChar      ()) a = "char literal";
    if(isLong      ()) a = "ulong literal";
    if(isDouble    ()) a = "double literal";
    if(isBool      ()) a = "bool literal";
    if(isIdentifier()) a = "identifier";
    if(isKeyword   ()) a = "keyword";

    return a + " \"" + OG_Value + "\"";
}