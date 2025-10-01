#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#define BUFF_SIZE 1024

char *shift(char ***xs, int *xs_sz) {
    assert((*xs_sz) > 0);
    char *arg = **xs;
    (*xs)++;
    (*xs_sz)--;
    return arg;
}

void cat_file(FILE *file) {
    char buffer[BUFF_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }
}

void cat_interactive(FILE *file) {
    char line[BUFF_SIZE];
    while (fgets(line, sizeof(line), file) != NULL) {
        fputs(line, stdout);
        fflush(stdout);
    }
}

int main(int argc, char **argv) {
    int exit_status = 0;
    shift(&argv, &argc); // program name

    if (argc == 0) {
        if (isatty(fileno(stdin))) {
            fflush(stdout);
            cat_interactive(stdin);
        } else {
            cat_file(stdin);
        }
        return 0;
    }

    while (argc > 0) {
        char *next_arg = shift(&argv, &argc);
        FILE *next_fptr = fopen(next_arg, "r");
        if (next_fptr == NULL) {
            fprintf(stderr, "cat: %s: No such file or directory\n", next_arg);
            exit_status = 1;
        } else {
            cat_file(next_fptr);
            fclose(next_fptr);
        }
    }

    return exit_status;
}
