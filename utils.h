#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*
    * Checks if user wishes to exit interactive mode.
    *
    * Parameters:
    *   input: user input
    * 
    * Returns:
    *   1 if input is "exit", 0 otherwise
*/
int exit_check(char* input);


/*
    * Strips leading and trailing whitespace from user input, in-place.
    *
    * Parameters:
    *   str: user input
*/
void strip_whitespace(char* str);


/*
    * Tokenizes user input, for use with execv. Caller is responsible for freeing memory,
    * function free_tokens() is provided for this purpose.
    *
    * Parameters:
    *   str: user input
    * 
    * Returns:
    *   tokens: array of string tokens, where the first token is the command and the rest are args
*/
char** tokenize(char* str);


/*
    * Frees memory allocated for tokens by tokenize().
    *
    * Parameters:
    *   tokens: array of string tokens
*/
void free_tokens(char** tokens);


/*
    * Combines an executable name with a path to create a full path to the executable.
    * Caller is responsible for freeing memory allocated for return value.
    * 
    * Parameters:
    *    path: path to use, i.e. /usr/bin/
    *    cmd: executable name, i.e. ls
*/
char* get_full_path(char* path, char* cmd);


/*
    * Checks if a file is executable.
    * 
    * Parameters:
    *    full_path: full path to file, i.e. /usr/bin/ls
    * 
    * Returns:
    *    1 if file is executable, 0 otherwise
*/
int is_executable(char* full_path);


/*
    * Searches for an executable in a list of paths.
    * Caller is responsible for freeing memory allocated for return value.
    * 
    * Parameters:
    *    paths: list of paths to search, i.e. {"/bin", "/usr/bin", NULL}
    *    tokens: array of string tokens, where the first token is the command and the rest are args
    *            for example, {"ls", "-l", NULL}
    * 
    * Returns:
    *    full_path: full path to executable, i.e. /usr/bin/ls
*/
char* get_exec_path(char** paths, char* cmd);
