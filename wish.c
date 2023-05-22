#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "utils.h"


int exit_check(char* line) {
    if (line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't') {
        return 1;
    }

    return 0;
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
