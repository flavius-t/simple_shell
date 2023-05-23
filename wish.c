#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "utils.h"


/*
    * Executes a command in a separate process.
    * 
    * Parameters:
    *    path: full path to executable, i.e. /usr/bin/ls
    *    tokens: array of string tokens, where the first token is the command and the rest are args
    *            for example, {"ls", "-l", NULL}
*/
void execute_input(char* path, char** tokens) {
    if (!path) {
        fprintf(stderr, "execute_input: no executable path provided\n");
        return;
    }

    if (!tokens) {
        fprintf(stderr, "execute_input: no tokens provided\n");
        return;
    }

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "failed to fork new process\n");
        return;
    } else if (rc == 0) {
        printf("starting child process: (pid:%d)\n", (int)getpid());

        execv(path, tokens);

        fprintf(stderr, "child process failed to execute command\n");
        exit(1);
    } else {
        int wc = wait(NULL);
        printf("Parent process (pid:%d): finished waiting for child process %d (rc_wait:%d) \n", (int)getpid(), rc, wc);
    }
}


void process_input(FILE* stream, char** paths) {
    char* input = NULL;
    size_t input_size = 0;

    while (1) {
        if (stream == stdin) {
            printf("wish> ");
        }

        int done = getline(&input, &input_size, stream);
        strip_whitespace(input);

        if (done == -1 || exit_check(input)) {
            break;
        }

        char** tokens = tokenize(input);
        if (!tokens) {
            fprintf(stderr, "failed to tokenize user input\n");
            continue;
        }

        char* exec_path = get_exec_path(paths, tokens[0]);
        execute_input(exec_path, tokens);

        free(exec_path);
        free_tokens(tokens);
    }

    free(input);
    input = NULL;
}


int main(int argc, char* argv[]) {
    // executable search paths
    char* paths[] = {"/bin", "usr/bin", NULL};

    if (argc == 1) {
        // interactive mode
        process_input(stdin, paths);
    } else if (argc == 2) {
        // batch mode
        char* filename = argv[1];
        strip_whitespace(filename);
        FILE* fp = fopen(filename, "r");
        if (!fp) {
            fprintf(stderr, "failed to open file %s\n", filename);
            return 1;
        }

        process_input(fp, paths);

        fclose(fp);
        fp = NULL;
        filename = NULL;
    } else {
        printf("Illegal number of args to wish\n");
    }

    for (int i = 0; paths[i] != NULL; i++) {
        paths[i] = NULL;
    }

    return 0;
}
