#include "grep.h"

void f_flags(grep_opt *options) {
  FILE *f = fopen(options->filename, "r");
  if (f) {
    while (!feof(f)) {
      if (fgets(options->filename, BUFF, f) != NULL) {
        strcat(options->str_arg, options->filename);
        strcat(options->str_arg, "|");
      }
    }
  }
}

void parser(int argc, char *argv[], grep_opt *options) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        options->e = 1;
        strcat(options->str_arg, optarg);
        strcat(options->str_arg, "|");
        break;

      case 'i':
        options->i = 1;

        break;

      case 'v':
        options->v = 1;

        break;

      case 'c':
        options->c = 1;

        break;

      case 'l':
        options->l = 1;

        break;

      case 'n':
        options->n = 1;

        break;

      case 'h':
        options->h = 1;

        break;

      case 's':
        options->s = 1;

        break;

      case 'f':
        options->f = 1;
        strcpy(options->filename, optarg);
        f_flags(options);

        break;

      case 'o':
        options->o = 1;

        break;
    }
  }
  if (!options->e && !options->f) {
    if (argc > optind) {
      strcat(options->str_arg, argv[optind]);
    }
    optind++;
  }
  if (options->e || options->f) {
    options->str_arg[strlen(options->str_arg) - 1] = '\0';
  }
  options->h_count = argc - 1;
}

void reader(char *argv[], grep_opt *options) {
  FILE *f = fopen(argv[optind], "r");
  regex_t reg;
  regmatch_t start;
  int match = 0;
  int c_counter = 0;
  int stroke_number = 0;
  int opt_i = REG_EXTENDED;
  if (options->i) {
    opt_i = REG_EXTENDED | REG_ICASE;
  }

  regcomp(&reg, options->str_arg, opt_i);
  if (f) {
    while (fgets(options->str, BUFF, f) != NULL) {
      stroke_number++;
      match = regexec(&reg, options->str, 1, &start, 0);
      if (!match && options->o) {
        char *start_pos = options->str;
        while (regexec(&reg, start_pos, 1, &start, 0) == 0) {
          char *end_pos = options->str + start.rm_so;
          if (options->n) printf("%d:", stroke_number);
          printf("%.*s\n", (int)(start.rm_eo - start.rm_so), end_pos);
          start_pos = end_pos + 1;
        }
      }
      if (!match && !options->h && options->h_count > optind)
        printf("%s:", argv[optind]);
      if (options->v) match = !match;
      if (!match && !options->c && !options->l && !options->n && !options->o)
        printf("%s", options->str);
      if (!match && options->n && !options->o)
        printf("%d:%s", stroke_number, options->str);
      if (!match) c_counter++;
      if (options->l && !match) printf("%s", argv[optind]);
    }
    if (options->c) printf("%d\n", c_counter);
    if (!match && stroke_number > 0 && !options->o) printf("\n");
    fclose(f);
  } else {
    if (!options->s)
      fprintf(stderr, "%s: No such file or directory\n", argv[optind]);
  }
  regfree(&reg);
}

int main(int argc, char *argv[]) {
  grep_opt options = {0};

  parser(argc, argv, &options);

  while (optind < argc) {
    reader(argv, &options);
    options.h_count++;
    optind++;
  }

  return 0;
}
