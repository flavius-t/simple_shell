#include "utils.h"


int exit_check(char* input) {
    if (input[0] == 'e' && input[1] == 'x' && input[2] == 'i' && input[3] == 't') {
        return 1;
    }

    return 0;
}


void strip_whitespace(char* str) {
    // remove leading whitespace
    while (isspace(*str)) {
        str++;
    }

    // remove trailing whitespace
    size_t len = strlen(str);
    while (len > 0 && isspace(str[len - 1])) {
        str[--len] = '\0';
    }

    return;
}


char** tokenize(char* str) {
    char** tokens = (char**)malloc(MAX_TOKENS * sizeof(char*));

    if (!tokens) {
        fprintf(stderr, "tokenize -- malloc failed\n");
        return NULL;
    }

    char* token = NULL;
    size_t i = 0;

    while ((token = strtok(str, " ")) != NULL) {
        size_t tokenLength = strlen(token);
        tokens[i] = (char*)malloc((tokenLength + 1) * sizeof(char));
        strcpy(tokens[i], token);
        i++;
        str = NULL;
    }

    tokens[i] = NULL; // indicates end of array for looping

    token = NULL;
    return tokens;

}


void free_tokens(char** tokens) {
    if (!tokens) {
        return;
    }

    for (size_t i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}


char* get_full_path(char* path, char* cmd) {
    size_t path_len = strlen(path);
    size_t cmd_len = strlen(cmd);

    // need enough room for both strings, plus a slash and a null terminator
    char* full_path = (char*)malloc((path_len + cmd_len + 2) * sizeof(char));

    if (!full_path) {
        fprintf(stderr, "get_exec_path -- malloc failed\n");
        return NULL;
    }

    strcpy(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, cmd);

    printf("full path: %s\n", full_path);

    return full_path;
}


int is_executable(char* full_path) {
    if (full_path && access(full_path, X_OK) == 0) {
        return 1;
    }

    return 0;
}


char* get_exec_path(char** paths, char* cmd) {
    char* full_path = NULL;

    for (size_t i = 0; paths[i] != NULL; i++) {
        full_path = get_full_path(paths[i], cmd);

        if (full_path && is_executable(full_path)) {
            break;
        }
    }

    return full_path;
}
