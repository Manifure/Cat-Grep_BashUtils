#ifndef GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define BUFF 8192

typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int h_count;
  char str[BUFF];
  char str_arg[BUFF];
  char filename[BUFF];
} grep_opt;

#endif