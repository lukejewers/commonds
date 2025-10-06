#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    OPTION_n,    // Print count lines of each of the specified files.
} Option;

typedef struct {
    Option option;
    int count;
    bool is_multiple_files;
} TailConfig;

void tail(FILE *fptr, const char *filename, TailConfig config) {
    if (config.is_multiple_files) printf("==> %s <==\n", filename);

    int total_lines = 0;
    int ch;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') total_lines++;
    }

    rewind(fptr);
    int lines_to_skip = total_lines - config.count;
    if (lines_to_skip < 0) lines_to_skip = 0;

    int current_line = 0;
    while ((ch = fgetc(fptr)) != EOF) {
        if (current_line >= lines_to_skip) {
            printf("%c", ch);
        }
        if (ch == '\n') current_line++;
    }

    if (config.is_multiple_files) printf("\n");
}

int main(int argc, char **argv) {
    TailConfig config = {0};
    config.option = OPTION_n;
    config.count = 10;
    bool option_found = false;

    if (argc > 1) {
        if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--lines") == 0) {
            config.option = OPTION_n;
            option_found = true;
        }

        if (option_found) {
            if (argc > 2) {
                char *endptr;
                long count = strtol(argv[2], &endptr, 10);
                if (*endptr != '\0' || count <= 0) {
                    fprintf(stderr, "tail: illegal line count -- %s\n", argv[2]);
                    return 1;
                }
                config.count = (int)count;
                argc -= 2;
                argv += 2;
            } else {
                fprintf(stderr, "tail: option requires an argument -- '%s'\n", argv[1]);
                return 1;
            }
        }
    }

    if (argc > 2) config.is_multiple_files = true;
    if (argc == 1) {
        fprintf(stderr, "tail: stdin not supported\n");
        return 1;
    }

    int exit_status = 0;
    for (int i = 1; i < argc; ++i) {
        const char *filename = argv[i];
        FILE *fptr = fopen(filename, "r");
        if (fptr == NULL) {
            fprintf(stderr, "tail: %s: No such file or directory\n", argv[i]);
            exit_status = 1;
        } else {
            tail(fptr, filename, config);
            fclose(fptr);
        }
    }
    return exit_status;
}
