



enum ErrorCode : int {
    SUCCESS = 0,



    ERROR_STRING_INCOMPLETE_n,               // String literal that doesn't have a closing " character (ends with \n)
    ERROR_STRING_INCOMPLETE_0,               // String literal that doesn't have a closing " character (ends with \0)

    ERROR_CHAR_INCOMPLETE_n,                 // Char literal that doesn't have a closing ' character (ends with \n)
    ERROR_CHAR_INCOMPLETE_0,                 // Char literal that doesn't have a closing ' character (ends with \0)

    // ERROR_ESCAPE_UNKNOWN,                    // Unknown escape sequence
    // ERROR_ESCAPE_U_EMPTY,                    // No digits in \u or \U escape sequence
    // ERROR_ESCAPE_U4_MISSING,                 // Missing digits in \u escape sequence
    // ERROR_ESCAPE_U4_MISSING,                 // Missing digits in \U escape sequence




    ERROR_INCLUDE_PATH_DIRECTORY,   // Path is a directory
    ERROR_INCLUDE_PATH_MISSING,     // Path is missing
    ERROR_INCLUDE_PATH_EMPTY,     // Path is empty
    ERROR_INCLUDE_PATH_CANNOT_OPEN,     // File cannot be opened (either missing or no permissions)
};