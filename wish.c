#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>


int exit_check(char* line) {
    if (line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't') {
        return 1;
    }

    return 0;
}


void execute_cmd(char* cmd) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        return;
    } else if (rc == 0) {
        printf("hello, I am child (pid:%d)\n", (int) getpid());

        // TODO: temporarily hardcode command and args; need to parse input str into command and separate args tokens
        char* path = "/usr/bin/ls";
        char* argv[] = { path, cmd, NULL };

        execv(path, argv);
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


int main(int argc, char* argv[]) {
    // check if interactive or batch mode
    if (argc == 1) {
        while (1) {
            printf("wish> ");

            // read line
            char* line = NULL;
            size_t line_size = 0;
            getline(&line, &line_size, stdin);

            // parse line: clean and process args
            strip_whitespace(line);

            if (exit_check(line)) {
                break;
            }

            execute_cmd(line);
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
