//TODO change output name
// > from Lynx/Tools/Build/Generators/ILSL
//exec          gcc ILSL.c Utils.c Tokenizer/Tokenizer.c Files.c -I. -std=c11 -o ../GenGlsl
//exec          gcc ILSL.c Utils.c Tokenizer/Tokenizer.c Files.c -I. -std=c11 -ggdb3 -g3 -O0 -o ../GenGlsl
// > from Lynx
//run           Tools/Build/Generators/GenGlsl src/Lynx/shaders/Volume.ilsl ../.engine/.tmp/glsl-Volume.ilsl.comp

//TODO MOVE TOOLS SOURCE FILES TO /src/Tools


//TODO remove useless includes
#include "Utils.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/stat.h>
#include <linux/limits.h>



//FIXME GLSL doesnt suppotr bool and integer matrices
//TODO check returns values
//TODO add macros
//TODO add include lists


//TODO remove useless includes
#include "Tokenizer/Tokens.h"
#include "Data/Constructs.h"

#include "Data/Operators.h"
#include "Data/Types.h"
#include "Data/Keywords.h"
#include "Data/Literals.h"




// Preprocessor ----------------------------------------------------------------------------------------------------------------------------//








/**
 * @brief Removes the trailing whitespace of each line
 *     Consecutive newline characters are preserved
 * @param vLines An array of Line structs containing the lines to clear
 * @param vNum The number of lines
 */
void clear(struct Line* vLines, const uint64_t vNum){
	for(uint64_t i = 0; i < vNum; ++i){
		struct Line* const l = &vLines[i];

		for(int64_t j = l->len - 1;; --j){
			//Lines containing only whitespace or empty lines
			if(j < 0){
				l->value[0] = '\0';
				l->len = 0;
				break;
			}
			// Lines with trailing whitespace or none
			else if(l->value[j] != ' ') {
			//!^ '\r' are removed, tabs are replaced with spaces and newlines are not part of the line value
				l->value[j + 1] = '\0';
				l->len = j;
				break;
			}
		}

	}
}
//TODO replace tabs with spaces




/**
 * @brief Replaces multiline comments with a space + a number of '\n' corresponding to the number of newlines they contain
 *     Single line comments are replaced with one '\n' character
 * @param vCode The code to uncomment
 * @param iFile The path of the file containing the code. This is used to print syntax errors
 * @return The resulting string
 */ //FIXME WRITE THE RESULT IN THE SAME ALLOCATION AND DONT RETURN IT
char* uncomment(const char* vCode, struct File* const iFile){
	char* code = malloc(MAX_CODE_LEN); code[0] = '\0';
	const uint64_t vCodeLen = strlen(vCode);
	uint64_t i = 0;


	//For each character
	while(i < vCodeLen){

		//If the character is a double quote
		if(vCode[i] == '"'){
			//Check if the string ends
			const char* const str_begin = vCode + i;
			int ends = 1;
			for(++i; vCode[i] != '"'; ++i){
				if(!vCode[i] || vCode[i] == '\n') {
					ends = 0; break;
				}
			}

			// If it does, paste it and skip closing '"'
			if(ends) {
				strncat(code, str_begin, i - (str_begin - vCode) + 1);
				++i;
			}
			// If it doesn't, print a syntax error
			else{
				uint64_t vLineN = 0; for(const char* c = vCode; c < str_begin; ++c) vLineN += *c == '\n';
				const char* errorLine = vLineN ? strchrn(vCode, '\n', vLineN - 1) : vCode;
				const char* iLineValue = strndup(errorLine, strchrn(vCode, '\n', vLineN) - errorLine);

				struct Line iLineInfo;
				iLineInfo.len     = strlen(iLineValue);
				iLineInfo.value   = strdup(iLineValue);
				iLineInfo.file    = iFile;
				iLineInfo.lineNum = vLineN;
				printSyntaxError(iLineInfo, "Unterminated string");
			}
		}

		// If it's not and there is enough space to start a comment
		else if(i < vCodeLen - 1){
			if(vCode[i] == '/' && vCode[i + 1] == '/'){			//If the character is the beginning of a single line comment
				i += 2;												//Ignore "//"
				while(i < vCodeLen && vCode[i] != '\n') ++i;		//Ignore the whole comment
				++i;												//Ignore '\n'
				strcat(code, "\n"); 								// Write '\n'
			}
			else if(vCode[i] == '/' && vCode[i + 1] == '*'){	//If the character is the beginning of a multiline comment
				strcat(code, " ");									//Add a space as token separator
				i += 2;												//Ignore "/*"
				while(i < vCodeLen && !(vCode[i] == '*' && vCode[i + 1] == '/')){ //For each character of the comment
					if(vCode[i] == '\n'){								//If the character is a newline
						strcat(code, "\n");									//Paste the newline
					}
					++i;												//Update the counter and ignore the other characters
				}
				i += 2;												//Ignore "*/"
			}
			else{												//Else
				strncat(code, &vCode[i], 1);						//Paste the character
				++i;												//Update the counter
			}
		}

		// Else paste whatever character was read
		else{
			strncat(code, &vCode[i], 1);
			++i;
		}
	}


	//Return the parsed code
	return code;
}
//FIXME dont use strcat and strncat or just start from an index




/**
 * @brief Checks if an included path is valid
 *     Prints an error if it's not
 * @param iLineValue The line value. Used to print errors
 * @param iLineNum The number of the line. Used to print errors
 * @param iFile The file path. Used to print errors
 * @param vPath The path of the included file
 */
void checkIncludeFile(const struct Line iLineInfo, const char* vPath){ //TODO check vLine type
	if(access(vPath, F_OK) == 0) {
		struct stat fileStat; stat(vPath, &fileStat);
		if(S_ISDIR(fileStat.st_mode)) {
			printSyntaxError(iLineInfo, "\"%s\" is a directory", vPath);
		}
	}
	else {
		printSyntaxError(iLineInfo, "No such file or directory");
	}
}




uint64_t statChar(const char* const vLine, const char vChar);
/**
 * @brief Checks if vLine is an include statement and returns the path of the included file
 * @param vLineValue The line to parse
 * @return The path of the included file, or NULL if the line is not an include statement
 */
char* isInclude(const struct Line iLineInfo){
	// Check #
	uint64_t i = countChar(iLineInfo.value, ' ');
	if(iLineInfo.value[i] != '#') return NULL;
	i += countChar(iLineInfo.value + ++i, ' ');

	// Check "include "
	const char* const include_value = "include ";
	const uint64_t include_len = strlen(include_value);
	if(memcmp(iLineInfo.value + i, include_value, include_len)) return NULL;
	i += include_len;

	// Get included file path
	char* const ret = malloc(PATH_MAX);				// Allocate space for the included file path
	const char c = iLineInfo.value[i] == '<' ? '>' : iLineInfo.value[i] == '"' ? '"' : '\0';
	if(c){												// If either '<' or '"' are used
		++i;												// Ignore '<' or '"'
		const uint64_t line_len = strlen(iLineInfo.value);		// Cache the length of the line
		for(uint64_t j = 0; i < line_len; ++i, ++j){		// Get the file path
			if(iLineInfo.value[i] == c) {						//
				ret[j] = '\0';								//
				return ret;									// Return the path
			}
			ret[j] = iLineInfo.value[i];
		}
	}
	printSyntaxError(iLineInfo, "Invalid include statement");
}




/**
 * @brief Creates a code with no includes by recursively pasting all the included files together
 *     Comments are not preserved
 * @param vCode The code containing the include statements
 * @param vFile The path of the file
 * @param vLineInfo The absolute line from which the file was included. UINT64_MAX if the file is not included
 * @param pLineNum The address of a uint64_t variable where to store the total number of lines
 * @return An array of Line structures of size *pNum containing the lines of all the included files
 */
struct Line* include(const char* const vFile, const uint64_t vFromLine, struct File* vFromFile, uint64_t* const pLineNum){
	//Reallocate file array
	files = reallocPow2(files, sizeof(struct File), filesNum);

	files[filesNum].path = strdup(vFile);
	files[filesNum].fromLine = vFromLine;
	files[filesNum].from = vFromFile;
	++filesNum;



	char* code = readFile(vFile, 4);									// Read the file
	code = uncomment(code, &files[filesNum - 1]);											// Uncomment it
	struct Line* const ret = malloc(sizeof(struct Line) * MAX_CODE_LINES);	// Allocate the return array



	char *line; uint64_t totLineNum = 0;
	for(uint64_t i = 0; (line = strsep(&code, "\n")) != NULL; ++i){			// For each line of the code
		// const uint64_t lineNum = vFromLine == UINT64_MAX ? i : vFromLine;		// Get the number of the line from which the file was included

		struct Line tmp_isinclude_info; //TODO
		tmp_isinclude_info.file = &files[filesNum - 1];
		tmp_isinclude_info.value = line;
		tmp_isinclude_info.len = strlen(line);
		tmp_isinclude_info.lineNum = i;
		char* const r = isInclude(tmp_isinclude_info);								// Check the line
		if(r != NULL){															// If the line is an include statement
			checkIncludeFile(tmp_isinclude_info, r);									// Check the included file
			uint64_t includedLen;													//
			struct Line* included = include(r, i, &files[filesNum - 1], &includedLen);// Get the lines of the included file
			memcpy(ret + totLineNum, included, sizeof(struct Line) * includedLen);	// Copy them in the return array
			free(included);															// Free the saved lines
			totLineNum += includedLen;												// Update the line counter
		}
		else{																	// If it's not
			// ret[totLineNum].lineNum = lineNum;										// Set the line numer
			ret[totLineNum].lineNum = i;										// Set the line numer
			ret[totLineNum].len     = strlen(line);									// Set the line length
			ret[totLineNum].value   = line;											// Set the line value
			ret[totLineNum].file    = &files[filesNum - 1];							// Set the line file
			++totLineNum;															// Update the line counter
		}
	}

	*pLineNum = totLineNum;
	return ret;
}







// Tokenizer -------------------------------------------------------------------------------------------------------------------------------//


#include "Tokenizer/Tokenizer.h"








// Syntax ----------------------------------------------------------------------------------------------------------------------------------//








void checkSyntax(const struct Token* const vTokens, const uint64_t vTokenNum, const struct Line* const iLines, const char* const iFileName){
	size_t curLine = 0;		// Current line number
	struct Scope g = {			// Global scope
		.functionc = 0,
		.strctc    = 0,
		.varc      = 0,
		.parent    = NULL
	};

	//! Whitespace is not saved as tokens
	for(size_t i = 0; i < vTokenNum;){
		if(isType(vTokens[i].id)){
			const struct Token* constructType = &vTokens[i++];
			if(i < vTokenNum && vTokens[i].id == e_user_defined) {
				const struct Token* constructName = &vTokens[i++];
				if(vTokens[i].id == o_rgroup){
					//TODO write recursive get-something functions
					//TODO getExpression takes the max number of tokens to check
				}
				else if(vTokens[i].id == o_log_eq || vTokens[i].value[0] == ';'){

				}
				else printSyntaxError(iLines[curLine], "Expected a function argument list or a variable definition after global identifier \"%s\"", constructName->value);
			}
			//FIXME check multiple definitions
			else printSyntaxError(iLines[curLine], "Expected identifier after type \"%s\"", constructType->value);
		}
		else if(vTokens[i].id == e_newline){
			++curLine; ++i;
		}
		else if(vTokens[i].id == e_preprocessor) {
			//FIXME actually check the syntax and save the arguments
			while(vTokens[i].id != e_newline) ++i;
		}
		else printSyntaxError(iLines[curLine], "Unexpected token \"%s\"", vTokens[i].value);
	}
}








// Output assembler -------------------------------------------------------------------------------------------------------------------------//








char* translate(const struct Token* vTokens, const uint64_t vTokensNum){
	char* const ret = malloc(MAX_CODE_LEN);
	uint64_t j = 0;
	for(uint64_t i = 0; i < vTokensNum; ++i){
		const struct Token* const curTok = vTokens + i;
		if(curTok->leading_ws){
			strcpy(ret + j, curTok->leading_ws);
			j += strlen(curTok->leading_ws);
		}

		if(curTok->id == e_literal){
			struct LiteralData_t* const tokData = curTok->data;
			char strValue[64];
			if(tokData->type == t_u32) snprintf(strValue, 64, "%d", *(uint32_t*)tokData->value);
			else                       snprintf(strValue, 64, "%lf",  *(double*)tokData->value);
			strcpy(ret + j, strValue);
			j += strlen(strValue);
		}
		else if(isType(curTok->id)){
			strcpy(ret + j, ((struct TypeData_t*)(curTok->data))->glslType);
			j +=     strlen(((struct TypeData_t*)(curTok->data))->glslType);
		}
		else if(isKeyword(curTok->id)){
			strcpy(ret + j, keywordValues[curTok->id - k_start]);
			j +=     strlen(keywordValues[curTok->id - k_start]);
		}
		else if(isOperator(curTok->id)){
			strcpy(ret + j, operatorValues[curTok->id - o_start]);
			j +=     strlen(operatorValues[curTok->id - o_start]);
		}
		else{
			strcpy(ret + j, curTok->value);
			j += curTok->len;
		}
	}
	ret[j + 1] = '\0';
	return ret;
}








// Main -------------------------------------------------------------------------------------------------------------------------------------//








void run(const char* const vSrc, const char* const vOut){
	files = malloc(sizeof(struct File) * 2);

	//Read input file
	const char* const src = realpath(vSrc, NULL); //Resolve symbolic links
	if(access(src, F_OK) != 0) printError("\"%s\": No such file or directory", vSrc);

	//Add hard coded version statement and parse the code
	uint64_t outputLinesNum;
	struct Line* const outputLines = include(vSrc, UINT64_MAX, NULL, &outputLinesNum);
	clear(outputLines, outputLinesNum);

	// Tokenize the code
	uint64_t outputTokensNum;
	struct Token* const outputTokens = tokenize(outputLines, outputLinesNum, &outputTokensNum, vSrc);

	// Check the syntax and write the GLSL code
	checkSyntax(outputTokens, outputTokensNum, outputLines, vSrc);
	char* const outputStr = translate(outputTokens, outputTokensNum);

	//Write output file
	FILE* ofile = fopen(vOut, "w");
	fprintf(ofile, "#version 450\n");
	fprintf(ofile, "%s", outputStr);
	fclose(ofile);
}






int32_t main(int32_t argc, char* argv[]){
	if(argc != 3){
		printf("GenGlsl: Wrong number of arguments");
		return 1;
	}

	run(argv[1], argv[2]);
	printf(argv[2]);
	return 0;
}