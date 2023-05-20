#include <stdio.h>


int exit_check(char* line) {
    if (line[0] == 'e' && line[1] == 'x' && line[2] == 'i' && line[3] == 't' && line[4] == '\n') {
        return 1;
    }

    return 0;
}


int main(int argc, char* argv[]) {
    // clean and process args

    // check if interactive or batch mode
    if (argc == 1) {
        while (1) {
            printf("wish> ");

            // read line
            char* line = NULL;
            size_t line_size = 0;
            getline(&line, &line_size, stdin);

            if (exit_check(line)) {
                break;
            }

            // parse line
            printf("line: %s", line);

            // execute line
        }
    } else if (argc == 2) {
        printf("Running batch mode with script: %s\n", argv[1]);
    } else {
        printf("Illegal number of args to wish\n");
    }

    return 0;
}
