#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

typedef enum {
    OPTION_NONE, // No option.
    OPTION_b,    // Number the non-blank output lines, starting at 1.
    OPTION_n,    // Number the output lines, starting at 1.
} Option;

void cat_file(FILE *file, Option option) {
    char line[BUFF_SIZE];
    switch (option) {
    case OPTION_b: {
        int line_count = 1;
        while (fgets(line, sizeof(line), file) != NULL) {
            if (line[0] == '\n') printf("\n");
            else printf("%6d\t%s", line_count++, line);
        }
        break;
    }
    case OPTION_n: {
        int line_count = 1;
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%6d\t%s", line_count++, line);
        }
        break;
    }
    default:
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
    }
}

int main(int argc, char **argv) {
    Option option = OPTION_NONE;
    bool has_stdin_arg = false;

    if (argc > 1) {
        if (strcmp(argv[1], "-b") == 0) option = OPTION_b;
        else if (strcmp(argv[1], "-n") == 0) option = OPTION_n;
        else if (strcmp(argv[1], "-") == 0 || strcmp(argv[1], "--") == 0) has_stdin_arg = true;
        if (option != OPTION_NONE) {
            argc--;
            argv++;
        }
    }

    if (argc == 1 || has_stdin_arg) {
        cat_file(stdin, option);
        return 0;
    }

    int exit_status = 0;
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
            exit_status = 1;
        } else {
            cat_file(file, option);
            fclose(file);
        }
    }

    return exit_status;
}
