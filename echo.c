#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *output = malloc(256 * sizeof(char));
  output[0] = '\0';
  char *separator = " ";
  char *ending = "\n";

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-n") == 0) {
      ending = "";
    } else {
	strcat(output, argv[i]);
	if (i < argc) {
	    strcat(output, separator);
	}
    }
  }
  printf("%s%s", output, ending);
  free(output);
  return 0;
}
