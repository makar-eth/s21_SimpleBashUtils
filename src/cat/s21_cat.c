#include "s21_cat.h"

int main(int argc, char **argv) {
  // print_args(argc, argv);
  if (argc > 1) {
    struct Options opt = {.b = 0,
                          .e = 0,
                          .n = 0,
                          .s = 0,
                          .t = 0,
                          .number = 0,
                          .number_nonblank = 0,
                          .squeeze_blank = 0};
    int n = parse_flags(argc, argv, &opt);
    open_files(argc, argv, n, opt);
    // print_opt(opt);
    // print_args(argc, argv);
  }
  return ERROR;
}

int parse_flags(int argc, char **argv, struct Options *opt) {
  int n = 0;
  for (int k = 1; k < argc; k++) {
    if (argv[k][0] == '-' && argv[k][1] == '-') {
      if (!strcmp(argv[k], "--number-nonblank")) {
        opt->number_nonblank = 1;
        argv[k] = "\0";
      } else if (!strcmp(argv[k], "--number")) {
        opt->number = 1;
        argv[k] = "\0";
      } else if (!strcmp(argv[k], "--squeeze-blank")) {
        opt->squeeze_blank = 1;
        argv[k] = "\0";
      } else {
        perror("Error1");
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
          case 't':
            opt->t = 1;
            break;
          case 'e':
            opt->e = 1;
            break;
          default:
            perror("Error2");
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
  while (n < argc) {
    f = fopen(argv[n], "r");
    if (f == NULL) {
      char str[150] = "";
      sprintf(str, "s21_cat: %s", argv[n]);
      perror(str);
    } else {
      print_content(f, opt);
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
      printf("%c", print);
      if (opt.b && print == '\n' && future != '\n') {
        line++;
        printf("%6d\t", line);
      }
      if (opt.n && print == '\n') {
        line++;
        printf("%6d\t", line);
      }
      if (opt.e && future == '\n') printf("$");
      print = future;
    }
  }
}

void print_opt(struct Options opt) {
  printf(
      "b = %d\nn = %d\ns = %d\ne = %d\nt = %d\nnumber = %d\nnumber-nonblank = "
      "%d\nsqueeze-blank = %d\n",
      opt.b, opt.n, opt.s, opt.e, opt.t, opt.number, opt.number_nonblank,
      opt.squeeze_blank);
}

void print_args(int argc, char **argv) {
  printf("%d\n", argc);
  for (int i = 1; i < argc; i++) {
    printf("%s\n", argv[i]);
  }
}