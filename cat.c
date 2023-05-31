#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10
#define MAX_LINE_LENGTH 1000

typedef struct {
    char *files[MAX_FILES];
    char *flag;
} Config;

void print_file(Config cfg) {
    FILE *file;
    for (int file_i = 0; cfg.files[file_i] != NULL; file_i++) {
      file = fopen(cfg.files[file_i], "r");
      if (file == NULL) {
	printf("File '%s' does not exist", cfg.files[file_i]);
	exit(EXIT_FAILURE);
      }
      char line[MAX_LINE_LENGTH];
      int line_number = 1;
      int blank_line_count = 0;
      while (fgets(line, sizeof(line), file) != NULL) {
	  if (strcmp(line, "\n") == 0) {
	      blank_line_count++;
	  }
	  if (strncmp(cfg.flag, "-n", 2) == 0) {
	      printf("%d %s", line_number, line);
	  } else if (strncmp(cfg.flag, "-b", 2) == 0) {
	      if (*line != '\n') {
		  printf("%d %s", line_number - blank_line_count, line);
	      } else {
		  printf("\n");
	      }
	  } else if (strstr(cfg.flag, "")) {
	      printf("%s", line);
	  }
	  line_number++;
      }
      fclose(file);
    }
}

Config parse_args(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: Please provide at least one file name\n");
    exit(EXIT_FAILURE);
  }
  if (argc - 1 > MAX_FILES) {
    printf("Error: Too many files. Maximum number of files allowed is %d\n", MAX_FILES);
    exit(EXIT_FAILURE);
  }

  Config cfg = { .files = { NULL }, .flag = "" };
  for (int arg_i = 0; arg_i < argc - 1; arg_i++) {
    char *arg = argv[arg_i + 1];
    if (strstr(arg, ".")) {
      cfg.files[arg_i] = arg;
    } else if (strstr(arg, "-")) {
	if (strncmp(arg, "-b", 2) == 0 || strncmp(arg, "-n", 2) == 0) {
	    cfg.flag = arg;
	} else {
	    printf("Error: Invalid flag '%s'. Please pass a valid flag (`-b`, `-n`)", arg);
	    exit(1);
	}
    }
  }
  return cfg;
}

int main(int argc, char *argv[]) {
  Config cfg = parse_args(argc, argv);
  print_file(cfg);
  return 0;
}
