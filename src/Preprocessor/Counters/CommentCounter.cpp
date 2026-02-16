#include "CommentCounter.hpp"
#include "Preprocessor/AnnotatedSource.hpp"








/**
 * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
 *     This function doesn't validate comments and doesn't print any error.
 *     Unterminated comments return the amount of characters from the opening sequence to the last available character.
 * @param b The string pipe that contains the comment.
 * @param index The index at which the comment starts.
 * @return The length of the comment, including the length of the opening and closing character sequences (not \0 or \n).
 *     If the buffer doesn't contain a comment that starts at index <index>, 0 is returned.
 */
ulong misc::measureComment(pre::AnnotatedSource<false> &b, ulong index) {
    if(b[index]->c != '/') return 0;


    char last = b[index]->c;
    char commType = '\0'; // '\0' if unknow, '/' if single line, '*' if multiline
    ulong i = index + 1;
    while(true) {

        // Single character closing sequences (End of file or single line comments)
        if(!b[i] || (commType == '/' && b[i]->c == '\n')) {
            break;
        }
        const char c = b[i]->c;

        // Starting sequence
        if(commType == '\0') {
            if(c == '/' || c == '*') {
                commType = c;
                continue;
            }
            else {  //! Starting sequence not found (this includes \n and \0 cases)
                return 0;
            }
        }

        // Double character closing sequences (Multi line comments)
        else if(commType == '*' && last == '*' && c == '/') {
            ++i;
            break;
        }

        // Normal characters (part of the comment)
        else {
            last = c;
            ++i;
        }
    }


    return i - index;
}



