#include <stdio.h>
#include <unistd.h>
#include <string.h>


void strip_whitespace(char* str);


/*
    * Tokenize user input, for use with execv.
    * Returns an array of tokens, where the first token is the command and the rest are args.
    * Returns NULL if malloc fails.
*/
char** tokenize(char* str);


void free_tokens(char** tokens);


char* get_full_path(char* path, char* command);


int is_executable(char* full_path);


char* get_exec_path(char** paths, char** tokens);
