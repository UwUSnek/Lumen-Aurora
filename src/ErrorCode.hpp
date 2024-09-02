#pragma once



enum ErrorCode : int {
    SUCCESS = 0,
    ERROR_OUTPUT_CANNOT_CREATE, // Output file cannot be created

    ERROR_CMD_SOURCE_MISSING,   // No source path specified in command
    ERROR_CMD_SOURCE_INVALID, // Source path could not be found
    ERROR_CMD_SOURCE_IS_DIRECTORY, // Source path is a directory
    ERROR_CMD_MULTIPLE_SOURCES, // More than one source path specified

    ERROR_CMD_NO_OUTPUT_FILE,   // No output path specified after output type option

    ERROR_CMD_UNKNOWN_OPTION, // Option not recognized
    ERROR_CMD_INCOMPATIBLE_OPTIONS, // 2 incompatible options are used together

    ERROR_CMD_INCLUDE_PATH_MISSING,
    ERROR_CMD_INCLUDE_PATH_INVALID,
    ERROR_CMD_INCLUDE_PATH_IS_FILE,

    ERROR_CMD_IMPORT_PATH_MISSING,
    ERROR_CMD_IMPORT_PATH_INVALID,
    ERROR_CMD_IMPORT_PATH_IS_FILE,




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




    ERROR_PRE_NO_PATH,     // Include path is missing
    ERROR_PRE_EMPTY_PATH,     // Include path is empty
    ERROR_PRE_PATH_IS_DIRECTORY,   // Include path is a directory
    ERROR_PRE_PATH_CANNOT_OPEN,     // Included file cannot be opened (either missing or no permissions)
};