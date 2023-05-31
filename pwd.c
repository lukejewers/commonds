#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *path = NULL;
    size_t size = 0;

    if (getcwd(NULL, 0) == NULL) {
	perror("Error getting current directory");
	exit(EXIT_FAILURE);
    }

    path = getcwd(NULL, size);
    if (path == NULL) {
	perror("Error getting current directory");
	exit(EXIT_FAILURE);
    }

    printf("%s\n", path);
    free(path);
    return EXIT_SUCCESS;
}
