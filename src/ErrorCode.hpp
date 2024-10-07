#pragma once








enum ErrorCode : int {
    //--------------------------------------------------------//
    //                     GENERIC ERRORS                     //
    //--------------------------------------------------------//

    SUCCESS = 0,
    ERROR_OUTPUT_CANNOT_CREATE,                 // Output file cannot be created
    ERROR_CODE_DISPLAY_NO_SOURCE,               // Source file of code display is not accessible anymore

    ERROR_PATH_IS_DIRECTORY,                    // Path is a directory
    ERROR_PATH_CANNOT_OPEN,                     // File cannot be opened (either missing or no permissions)







    //--------------------------------------------------------//
    //                     COMMAND PARSING                    //
    //--------------------------------------------------------//

    ERROR_CMD_SOURCE_MISSING,                   // No source path specified in command
    ERROR_CMD_SOURCE_INVALID,                   // Source path could not be found
    ERROR_CMD_SOURCE_IS_DIRECTORY,              // Source path is a directory
    ERROR_CMD_MULTIPLE_SOURCES,                 // More than one source path specified

    ERROR_CMD_NO_OUTPUT_FILE,                   // No output path specified after output type option

    ERROR_CMD_UNKNOWN_OPTION,                   // Option not recognized
    ERROR_CMD_INCOMPATIBLE_OPTIONS,             // 2 incompatible options are used together

    ERROR_CMD_INCLUDE_PATH_MISSING,             // Missing include path
    ERROR_CMD_INCLUDE_PATH_NOT_FOUND,           // Include path doesn't exist
    ERROR_CMD_INCLUDE_PATH_NO_PERMISSION,       // No permission to access the include path
    ERROR_CMD_INCLUDE_PATH_IS_FILE,             // Include path is a file

    ERROR_CMD_IMPORT_PATH_MISSING,              // Missing import path
    ERROR_CMD_IMPORT_PATH_NOT_FOUND,            // import path doesn't exist
    ERROR_CMD_IMPORT_PATH_NO_PERMISSION,        // No permission to access the import path
    ERROR_CMD_IMPORT_PATH_IS_FILE,              // import path is a file








    //--------------------------------------------------------//
    //                      PREPROCESSOR                      //
    //--------------------------------------------------------//

    ERROR_PRE_NO_PATH,                          // Include path is missing
    ERROR_PRE_EMPTY_PATH,                       // Include path is empty
    ERROR_PRE_PATH_IS_DIRECTORY,                // Include path is a directory
    ERROR_PRE_PATH_NOT_FOUND,                   // Included file doesn't exist
    ERROR_PRE_PATH_NO_PERMISSION,               // Included file has no read permission
    ERROR_PRE_PATH_AMBIGUOUS,                   // Included path can point to multiple files in different include paths








    //--------------------------------------------------------//
    //               COMPILATION - TOKENIZATION               //
    //--------------------------------------------------------//

    ERROR_CMP_CHARACTER_INVALID,                // Invalid character found in the source code

    ERROR_CMP_STRING_INCOMPLETE_n,              // String literal that doesn't have a closing " character (ends with \n)
    ERROR_CMP_STRING_INCOMPLETE_0,              // String literal that doesn't have a closing " character (ends with \0)

    ERROR_CMP_CHAR_INCOMPLETE_n,                // Char literal that doesn't have a closing ' character (ends with \n)
    ERROR_CMP_CHAR_INCOMPLETE_0,                // Char literal that doesn't have a closing ' character (ends with \0)
    ERROR_CMP_CHAR_EMPTY,                       // Char literal contains 0 bytes
    ERROR_CMP_CHAR_LONG,                        // Char literal contains more than 1 byte

    ERROR_CMP_ESCAPE_INVALID,                   // Unknown escape sequence
    ERROR_CMP_ESCAPE_U_EMPTY,                   // No digits in \u or \U escape sequence
    ERROR_CMP_ESCAPE_U4_MISSING,                // Missing digits in \u escape sequence
    ERROR_CMP_ESCAPE_U8_MISSING,                // Missing digits in \U escape sequence

    ERROR_CMP_LITERAL_BASE_INVALID,             // Invalid base prefix (not 0b, 0o, 0d or 0x)
    ERROR_CMP_LITERAL_DIGITS_MISSING,           // No digits fuond afer base prefix
    ERROR_CMP_LITERAL_DIGITS_INVALID,           // Digit is not allowed by the numerical base








    //--------------------------------------------------------//
    //                COMPILATION - TREE PHASE                //
    //--------------------------------------------------------//

    ERROR_CMP_UNEXPECTED_TOKEN, // Unexpected token found during tree creation
    ERROR_CMP_UNEXPECTED_END,   // Code ends before a semantic element's syntax is complete
    ERROR_CMP_MISSING_TOKEN,   // A token that was expected is missing

    ERROR_CMP_SCOPE_MISSING_INITIATOR,            // Missing {
    ERROR_CMP_SCOPE_MISSING_TERMINATOR,            // Missing }
    ERROR_CMP_SCOPE_CHILD_NOT_ALLOWED,   // Target scope cannot contain the element

    ERROR_CMP_PATH_NO_IDENTIFIER,   // No identifier after . keyword


    ERROR_CMP_ALIAS_NO_PATH,   // Alias path is missing
    ERROR_CMP_ALIAS_NO_AS,   // Alias path is missing
    ERROR_CMP_ALIAS_NO_NAME,   // Alias path is missing

    ERROR_CMP_NAMESPACE_NAME_MISSING,  // Scope {} is missing
    ERROR_CMP_NAMESPACE_SCOPE_MISSING,  // Scope {} is missing
};