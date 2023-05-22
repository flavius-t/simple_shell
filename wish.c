#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 64


int exit_check(char* line) {
    if (line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't') {
        return 1;
    }

    return 0;
}


char* get_full_path(char* path, char* command) {
    size_t path_len = strlen(path);
    size_t command_len = strlen(command);

    // need enough room for both strings, plus a slash and a null terminator
    char* full_path = (char*)malloc((path_len + command_len + 2) * sizeof(char));

    if (!full_path) {
        fprintf(stderr, "get_exec_path -- malloc failed\n");
        return NULL;
    }

    strcpy(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, command);

    printf("full path: %s\n", full_path);

    return full_path;
}


int is_executable(char* full_path) {
    if (full_path && access(full_path, X_OK) == 0) {
        return 1;
    }

    return 0;
}


char* get_exec_path(char** paths, char** tokens) {
    char* full_path = NULL;

    for (size_t i = 0; paths[i] != NULL; i++) {
        full_path = get_full_path(paths[i], tokens[0]);

        if (full_path && is_executable(full_path)) {
            printf("found executable: %s\n", paths[i]);

            break;
        }
    }

    return full_path;
}


void execute_input(char* path, char** tokens) {
    if (!path) {
        fprintf(stderr, "execute_input -- no executable path\n");
        return;
    }

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        return;
    } else if (rc == 0) {
        printf("hello, I am child (pid:%d)\n", (int) getpid());

        execv(path, tokens);

        fprintf(stderr, "child_process failed to execute command\n");
        exit(1);

    } else {
        int wc = wait(NULL);
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, wc, (int) getpid());
    }
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


/*
    * Tokenize user input, for use with execv.
    * Returns an array of tokens, where the first token is the command and the rest are args.
    * Returns NULL if malloc fails.
*/
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


int main(int argc, char* argv[]) {
    char* paths[] = {"/bin", "usr/bin", NULL};

    // check if interactive or batch mode
    if (argc == 1) {
        char* line = NULL;
        size_t line_size = 0;

        while (1) {
            printf("wish> ");

            getline(&line, &line_size, stdin);

            strip_whitespace(line);

            if (exit_check(line)) {
                free(line);
                line = NULL;
                break;
            }

            char** tokens = tokenize(line);

            if (!tokens) {
                fprintf(stderr, "execute_input -- no tokens\n");
                break;
            }

            char* exec_path = get_exec_path(paths, tokens);

            if (!exec_path) {
                fprintf(stderr, "execute_input -- no executable found\n");
                break;
            }

            execute_input(exec_path, tokens);

            free(exec_path);

            free_tokens(tokens);
        }
    } else if (argc == 2) {
        printf("Running batch mode with script: %s\n", argv[1]);
        // clean and process args

        // run each arg sequentially (not concurrently) in a new process
    } else {
        printf("Illegal number of args to wish\n");
    }

    return 0;
}
