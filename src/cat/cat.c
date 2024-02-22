#include "cat.h"

void parser(int argc, char *argv[], cat_opt *options) {
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}

  };
  int opt;
  while ((opt = getopt_long(argc, argv, "+benstvTE", long_options, 0)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;

        break;

      case 'e':
        options->e = 1;
        options->v = 1;

        break;

      case 'E':
        options->e = 1;

        break;

      case 'n':
        options->n = 1;

        break;

      case 's':
        options->s = 1;

        break;

      case 't':
        options->t = 1;
        options->v = 1;

        break;

      case 'T':
        options->t = 1;

        break;

      case 'v':
        options->v = 1;

        break;
    }
  }
}

void reader(char *argv[], cat_opt *options) {
  FILE *f = fopen(argv[optind], "r");
  if (f) {
    int cur;
    int last = '\n';
    int str_count = 0;

    while ((cur = fgetc(f)) != EOF) {
      if ((options->b && cur != '\n') || options->n) {
        if (last == '\n') {
          printf("%6d\t", ++str_count);
        }
      }
      if (options->s) {
        if (cur == '\n' && last == '\n') {
          str_count++;
          if (str_count > 1) {
            continue;
          }
        } else {
          str_count = 0;
        }
      }
      if (options->e && cur == '\n') {
        printf("$");
      }
      if (options->t && cur == '\t') {
        printf("^I");
        continue;
      }
      if (options->v) {
        if ((cur >= 0 && cur < 32) && (cur != 9 && cur != 10)) {
          printf("^");
          cur += 64;
        }
        if (cur >= 127 && cur <= 159) {
          if (cur == 127) {
            printf("^");
          } else {
            printf("M-^");
          }
          cur -= 64;
        }
      }

      printf("%c", cur);
      last = cur;
    }
    fclose(f);

  } else {
    fprintf(stderr, "%s: No such file or directory\n", argv[optind]);
  }
}

int main(int argc, char *argv[]) {
  cat_opt options = {0};

  parser(argc, argv, &options);

  while (optind < argc) {
    reader(argv, &options);
    optind++;
  }

  return 0;
}
