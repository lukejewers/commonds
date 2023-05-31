#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILES 8
#define MAX_LINE_LENGTH 1024

typedef struct {
    char *files[MAX_FILES];
    char *flag;
    int number;
} Config;

void print_lines(FILE *fptr, int number) {
    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (fgets(line, sizeof(line), fptr) != NULL) {
	if (i < number) {
	    printf("%s", line);
	}
	i++;
    }
    fclose(fptr);
}

void print_bytes(FILE *fptr, int number) {
    char line[MAX_LINE_LENGTH];
    fread(line, 1, number, fptr);
    printf("%s", line);
}

void print_file(Config cfg) {
  int file_count = 0;
  while (cfg.files[file_count] != NULL) {
      file_count++;
  }
  for (int i = 0; i < file_count; i++) {
    FILE *fptr = fopen(cfg.files[i], "r");
    if (fptr == NULL) {
      fprintf(stderr, "Error: File '%s' does not exist\n", cfg.files[i]);
      exit(1);
    }
    if (file_count > 1) {
      printf("==> %s <==\n", cfg.files[i]);
    }
    if (strcmp(cfg.flag, "-n") == 0 || cfg.flag == NULL) {
      print_lines(fptr, cfg.number);
    } else if (strcmp(cfg.flag, "-c") == 0) {
      print_bytes(fptr, cfg.number);
    }
    if (file_count > 1 && file_count - 1 != i) {
      printf("\n");
    }
  }
}

Config parse_args(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Error: Please pass at least one file\n");
    exit(1);
  }
  Config cfg = { .files = { NULL }, .flag = "-n", .number = 10 };
  for (int i = 1; i < argc; i++) {
    if (strstr(argv[i], ".") && i-1 >= 0) {
      cfg.files[i-1] = argv[i];
    } else if (strstr(argv[i], "-")) {
      if (strstr(argv[i], "-n") || strstr(argv[i], "-c")) {
	cfg.flag = argv[i];
      } else {
	fprintf(stderr, "Error: Please pass a valid flag (`-n`, `-c`)");
	exit(1);
      }
    }
    if (isdigit(*argv[i])) {
      cfg.number = atoi(argv[i]);
    }
  }
  if (strlen(cfg.flag) == 0) cfg.flag = "";
  if (cfg.number == 0) cfg.number = 10;
  if (strlen(cfg.flag) == 0 && cfg.number != 10) {
    fprintf(stderr, "Error: Please pass a valid flag [-n, -c] and number [number])\n");
    exit(1);
  }
  return cfg;
}

int main(int argc, char *argv[]) {
  Config cfg = parse_args(argc, argv);
  print_file(cfg);
  return 0;
}
