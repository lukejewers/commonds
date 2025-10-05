#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    OPTION_c,    // Print bytes of each of the specified files.
    OPTION_n,    // Print count lines of each of the specified files.
} Option;

typedef struct {
    Option option;
    int count;
    bool is_multiple_files;
} HeadConfig;

void head(FILE *fptr, const char *filename, HeadConfig config) {
    int ch;
    int line_count = 0, bytes_count = 0;
    if (config.is_multiple_files) printf("==> %s <==\n", filename);
    while ((ch = fgetc(fptr)) != EOF) {
        if (config.option == OPTION_c && bytes_count == config.count) break;
        if (config.option == OPTION_n && line_count  == config.count) break;
        bytes_count++;
        if (ch == '\n') line_count++;
        printf("%c", ch);
    }
    if (config.is_multiple_files) printf("\n");
}

int main(int argc, char **argv) {
    HeadConfig config = {0};
    config.option = OPTION_n;
    config.count = 10;
    bool option_found = false;

    if (argc > 1) {
        if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--bytes") == 0) {
            config.option = OPTION_c;
            option_found = true;
        } else if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--lines") == 0) {
            config.option = OPTION_n;
            option_found = true;
        }

        if (option_found) {
            if (argc > 2) {
                char *endptr;
                long count = strtol(argv[2], &endptr, 10);
                if (*endptr != '\0' || count <= 0) {
                    fprintf(stderr, "head: illegal line count -- %s\n", argv[2]);
                    return 1;
                }
                config.count = (int)count;
                argc -= 2;
                argv += 2;
            } else {
                fprintf(stderr, "head: option requires an argument -- '%s'\n", argv[1]);
                return 1;
            }
        }
    }

    if (argc > 2) config.is_multiple_files = true;
    if (argc == 1) {
        head(stdin, "", config);
        return 0;
    }

    int exit_status = 0;
    for (int i = 1; i < argc; ++i) {
        const char *filename = argv[i];
        FILE *fptr = fopen(filename, "r");
        if (fptr == NULL) {
            fprintf(stderr, "head: %s: No such file or directory\n", argv[i]);
            exit_status = 1;
        } else {
            head(fptr, filename, config);
            fclose(fptr);
        }
    }
    return exit_status;
}
