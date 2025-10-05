#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    OPTION_NONE, // No option.
    OPTION_c,    // The number of bytes in each input file is written to the standard output.
    OPTION_l,    // The number of lines in each input file is written to the standard output.
    OPTION_w,    // The number of words in each input file is written to the standard output.
} Option;

void wc(FILE *fptr, const char* filename, Option option) {
    int line_count = 0, word_count = 0, byte_count = 0;
    bool in_word = false;

    int ch;
    while ((ch = fgetc(fptr)) != EOF) {
        byte_count++;
        if (ch == '\n') line_count++;
        if (isspace(ch)) {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }

    }

    switch (option) {
    case OPTION_c:
        printf("%7d %s\n", byte_count, filename);
        break;
    case OPTION_l:
        printf("%7d %s\n", line_count, filename);
        break;
    case OPTION_w:
        printf("%7d %s\n", word_count, filename);
        break;
    case OPTION_NONE:
    default:
        printf("%7d %7d %7d %s\n", line_count, word_count, byte_count, filename);
        break;
    }
}

int main(int argc, char **argv) {
    Option option = OPTION_NONE;

    if (argc > 1) {
        if (strcmp(argv[1], "-c") == 0) option = OPTION_c;
        else if (strcmp(argv[1], "-l") == 0) option = OPTION_l;
        else if (strcmp(argv[1], "-w") == 0) option = OPTION_w;
        if (option != OPTION_NONE) {
            argc--;
            argv++;
        }
    }

    if (argc == 1) {
        wc(stdin, "", option);
        return 0;
    }

    int exit_status = 0;
    for (int i = 1; i < argc; ++i) {
        const char* filename = argv[i];
        FILE *fptr = fopen(argv[i], "r");
        if (fptr == NULL) {
            fprintf(stderr, "wc: %s: No such file or directory\n", argv[i]);
            exit_status = 1;
        } else {
            wc(fptr, filename, option);
            fclose(fptr);
        }
    }
    return exit_status;
}
