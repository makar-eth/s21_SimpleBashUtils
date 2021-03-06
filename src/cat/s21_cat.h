#include <stdio.h>
#include <string.h>

#ifndef ERROR
#define ERROR 0
#endif

struct Options {
  int b;
  int s;
  int e;
  int t;
  int n;
  int v;
};

void print_opt(struct Options opt);
int parse_flags(int argc, char **argv, struct Options *opt, int *error);
void open_files(int argc, char **argv, int n, struct Options opt);
void print_content(FILE *f, struct Options opt);
void print_args(int argc, char **argv);