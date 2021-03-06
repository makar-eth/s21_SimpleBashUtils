#include "s21_cat.h"

int main(int argc, char **argv) {
  int error = 0;
  if (argc > 1) {
    struct Options opt = {.b = 0,
                          .e = 0,
                          .n = 0,
                          .s = 0,
                          .t = 0,
                          .v = 0,
    };
    int n = parse_flags(argc, argv, &opt, &error);
    if (!error) {
      open_files(argc, argv, n, opt);
    }
  }
  return error;
}

int parse_flags(int argc, char **argv, struct Options *opt, int *error) {
  int n = 0;
  for (int k = 1; k < argc; k++) {
    if (argv[k][0] == '-' && argv[k][1] == '-') {
      if (!strcmp(argv[k], "--number-nonblank")) {
        opt->b = 1;
        argv[k] = "\0";
      } else if (!strcmp(argv[k], "--number")) {
        opt->n = 1;
        argv[k] = "\0";
      } else if (!strcmp(argv[k], "--squeeze-blank")) {
        opt->s = 1;
        argv[k] = "\0";
      } else {
        *error = 1;
        fprintf(stderr, "s21_cat: illegal option -- -\nusage: s21_cat [-benstuv] [file ...]\n");
      }
    } else if (argv[k][0] == '-') {
      for (int i = 1; i < strlen(argv[k]); i++) {
        switch (argv[k][i]) {
          case 'b':
            opt->b = 1;
            break;
          case 'n':
            opt->n = 1;
            break;
          case 's':
            opt->s = 1;
            break;
          case 'v':
            opt->v = 1;
            break;
          case 't':
            opt->t = 1;
            opt->v = 1;
            break;
          case 'e':
            opt->e = 1;
            opt->v = 1;
            break;
          case 'E':
            opt->e = 1;
            break;
          case 'T':
            opt->t = 1;
            break;
          default:
            *error = 1;
            fprintf(stderr, "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n", argv[k][i]);
        }
      }
    } else {
      n = k;
      break;
    }
  }
  if (opt->b) opt->n = 0;
  return n;  // 0 - if no files, else [index] there filenames starts
}

void open_files(int argc, char **argv, int n, struct Options opt) {
  FILE *f;
  int flag = 0;
  if (flag == 1) printf("\t");
  while (n < argc) {
    f = fopen(argv[n], "r");
    if (f == NULL) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", argv[n]);
      break;
    } else {
      print_content(f, opt);
      flag = 1;
      fclose(f);
    }
    n++;
  }
}

void print_content(FILE *f, struct Options opt) {
  char future = '\0';
  char print = fgetc(f);
  if (print != EOF) {
    int line = 1;
    int flag = 1;
    if (opt.b && print != '\n') printf("%6d\t", line);
    if (opt.n) printf("%6d\t", line);
    if (opt.e && print == '\n') printf("$");
    while (future != EOF) {
      future = fgetc(f);
      if (opt.s && print == '\n' && future == '\n') {
          while (future == '\n') {future = fgetc(f);}
          if(!flag) {
            printf("\n");
            if (opt.n) {
              line++;
              printf("%6d\t", line);
            }
            if (opt.e) printf("$");
          }
      }
      flag = 0;
      if (opt.v && (0 <= (int)print) && ((int)print < 32) && print != '\n'&& print != '\t') {
        printf("^%c", (int)print + 64);
      } else if ((int)print == 127 && opt.v) {
        printf("^%c", (int)print - 64);
      } else if (opt.t && print == '\t') {
        printf("^I");
      } else {
        printf("%c", print);
      }
      if ((opt.b && print == '\n' && future != '\n') || (opt.n && print == '\n') && future != EOF) {
        if (line != 1) line++;
        printf("%6d\t", line);
      }
      if (opt.e && future == '\n') printf("$");
      print = future;
    }
  }
}

void print_opt(struct Options opt) {
  printf(
      "b = %d\nn = %d\ns = %d\nv = %d\ne = %d\nt = %d\n",
      opt.b, opt.n, opt.s, opt.v, opt.e, opt.t);
}

void print_args(int argc, char **argv) {
  printf("%d\n", argc);
  for (int i = 1; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
}