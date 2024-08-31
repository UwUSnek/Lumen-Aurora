#pragma once



enum ErrorCode : int {
    SUCCESS = 0,

    ERROR_CMD_NO_SOURCE_FILE,
    ERROR_CMD_NO_OUTPUT_FILE,
    ERROR_CMD_UNKNOWN_OPTION,
    ERROR_CMD_INCOMPATIBLE_OPTIONS,




    ERROR_PATH_IS_DIRECTORY,   // Path is a directory
    ERROR_PATH_CANNOT_OPEN,     // File cannot be opened (either missing or no permissions)





    ERROR_STRING_INCOMPLETE_n,               // String literal that doesn't have a closing " character (ends with \n)
    ERROR_STRING_INCOMPLETE_0,               // String literal that doesn't have a closing " character (ends with \0)

    ERROR_CHAR_INCOMPLETE_n,                 // Char literal that doesn't have a closing ' character (ends with \n)
    ERROR_CHAR_INCOMPLETE_0,                 // Char literal that doesn't have a closing ' character (ends with \0)

    // ERROR_ESCAPE_UNKNOWN,                    // Unknown escape sequence
    // ERROR_ESCAPE_U_EMPTY,                    // No digits in \u or \U escape sequence
    // ERROR_ESCAPE_U4_MISSING,                 // Missing digits in \u escape sequence
    // ERROR_ESCAPE_U4_MISSING,                 // Missing digits in \U escape sequence




    ERROR_PRE_NO_PATH,     // Path is missing
    ERROR_PRE_EMPTY_PATH,     // Path is empty
    ERROR_PRE_PATH_IS_DIRECTORY,   // Path is a directory
    ERROR_PRE_PATH_CANNOT_OPEN,     // File cannot be opened (either missing or no permissions)
};