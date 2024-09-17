#include "TokenizedSource.hpp"




std::map<std::string, cmp::TokenType> cmp::keywordMap  = {
    { "namespace", TokenType::KEYWORD_NAMESPACE },
    { "enum",      TokenType::KEYWORD_ENUM      },
    { "struct",    TokenType::KEYWORD_STRUCT    },
    { "const",     TokenType::KEYWORD_CONST     },
    { "auto",      TokenType::KEYWORD_AUTO      },
    { "typeof",    TokenType::KEYWORD_TYPEOF    },
    { "alias",     TokenType::KEYWORD_ALIAS     },
    { "from",      TokenType::KEYWORD_FROM      },
    { "import",    TokenType::KEYWORD_IMPORT    },
    { "export",    TokenType::KEYWORD_EXPORT    },
    { "true",      TokenType::KEYWORD_TRUE      },
    { "false",     TokenType::KEYWORD_FALSE     },

    { "@",         TokenType::KEYWORD_PTR       },
    { "$",         TokenType::KEYWORD_ADDR      },
    { "if",        TokenType::KEYWORD_IF        },
    { "else",      TokenType::KEYWORD_ELSE      },
    { "for",       TokenType::KEYWORD_FOR       },
    { "while",     TokenType::KEYWORD_WHILE     },
    { "do",        TokenType::KEYWORD_DO        },
    { "continue",  TokenType::KEYWORD_CONTINUE  },
    { "break",     TokenType::KEYWORD_BREAK     },
    { "once",      TokenType::KEYWORD_ONCE      },
    { "then",      TokenType::KEYWORD_THEN      },
    { "return",    TokenType::KEYWORD_RETURN    },
    { "delay",     TokenType::KEYWORD_DELAY     },
    { "async",     TokenType::KEYWORD_ASYNC     },




    { "(",         TokenType::KEYWORD_ROUND_L    },
    { ")",         TokenType::KEYWORD_ROUND_R    },
    { "{",         TokenType::KEYWORD_CURLY_L    },
    { "}",         TokenType::KEYWORD_CURLY_R    },

    { ",",         TokenType::KEYWORD_COMMA      },
    { ";",         TokenType::KEYWORD_SEMICOLON  },
    { ".",         TokenType::KEYWORD_MEMBER     },
    { "::",        TokenType::KEYWORD_REFLECTION },




    { "template",  TokenType::META_KEYWORD_TEMPLATE },
    { "<",         TokenType::META_KEYWORD_LT       },
    { ">",         TokenType::META_KEYWORD_GT       },
    { "type",      TokenType::META_KEYWORD_TYPE     },
    { "!",         TokenType::META_KEYWORD_TARGET   },
    { "?",         TokenType::META_KEYWORD_ATS      },
    { "when",      TokenType::META_KEYWORD_WHEN     },
    { "priority",  TokenType::META_KEYWORD_PRIORITY },
    { "as",        TokenType::META_KEYWORD_AS       },
    { "to",        TokenType::META_KEYWORD_TO       },
    { "=",         TokenType::META_KEYWORD_ASSIGN   },
    { ":",         TokenType::META_KEYWORD_BASE     },
};