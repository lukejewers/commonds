#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

char *shift(char ***xs, int *xs_sz) {
    assert((xs_sz) > 0);
    char *arg = **xs;
    (*xs)++;
    (*xs_sz)--;
    return arg;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("\n"); // echo prints a new line when no args
        return 0;
    }
    shift(&argv, &argc); // shift program name
    int total_num_args = argc;
    int has_newline_flag = false;
    while (argc) {
        char *arg = shift(&argv, &argc);
        char *sep = " ";
        if (argc == total_num_args - 1 && strcmp(arg, "-n") == 0) {
            if (argc == 0) return 0;
            has_newline_flag = true;
            continue;
        }
        if (argc == 0 && has_newline_flag) sep = "";
        else if (argc == 0) sep = "\n";
        printf("%s", arg);
        printf("%s", sep);
    }
    return 0;
}
