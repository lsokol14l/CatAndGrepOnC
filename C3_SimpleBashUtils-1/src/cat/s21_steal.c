#include "s21_steal.h"

int main(int argc, char *argv[]) {
  options_t options = {0};

  if (parse_options(argc, argv, &options) != 0) {
    fprintf(stderr, "Usage: %s [OPTION]... [FILE]...\n%s", argv[0],
            CAT_OPTIONS_MANUAL);
    return EXIT_FAILURE;
  }

  int line_number = 1;

  if (optind >= argc) {
    // Если файлы не указаны, читаем из stdin
    process_file("-", options, &line_number);
  } else {
    for (int i = optind; i < argc; i++) {
      process_file(argv[i], options, &line_number);
    }
  }

  return EXIT_SUCCESS;
}

int parse_options(int argc, char *argv[], options_t *options) {
  int opt_index = 0;
  int opt;
  int flag = 0;
  struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 'e'},
      {0, 0, 0, 'E'},
      {0, 0, 0, 't'},
      {0, 0, 0, 'T'},
      {0, 0, 0, 'v'},
      {0, 0, 0, 0}  // терминатор
  };

  while ((opt = getopt_long(argc, argv, "benstEvT", long_options,
                            &opt_index)) != -1) {
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
      default:
        flag = -1;
    }
  }

  return flag;
}

void process_file(const char *filename, options_t options,
                  int *line_number_ptr) {
  FILE *file;
  if (strcmp(filename, "-") == 0) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
    if (file == NULL) {
      fprintf(stderr, "%s: %s: No such file or directory\n", "s21_cat",
              filename);
      return;
    }
  }

  char buffer[BUFFER_SIZE];
  int prev_was_blank = 0;
  int len;

  while ((len = read_line(file, buffer, sizeof(buffer))) != -1) {
    int current_is_blank = is_blank_line(buffer, len);

    // обработка флага squeeze lines
    if (options.s) {
      if (current_is_blank) {
        if (prev_was_blank) {
          continue;
        }
        prev_was_blank = 1;
      } else {
        prev_was_blank = 0;
      }
    }
    // обработка флага number-nonblank > number
    if (options.b && !current_is_blank) {
      printf("%6d\t", (*line_number_ptr)++);
    } else if (options.n && !options.b) {
      printf("%6d\t", (*line_number_ptr)++);
    }

    // обрабатываем флаги для отображения спец символов
    if (options.e || options.t || options.v) {
      process_line(buffer, len, options);
    } else {
      fwrite(buffer, 1, len, stdout);
    }
  }

  if (file != stdin) {
    fclose(file);
  }
}

void process_line(char *line, int len, options_t options) {
  for (int i = 0; i < len; i++) {
    unsigned char c = (unsigned char)line[i];
    if (c == '\t' && options.t) {
      printf("^I");
    } else if (c == '\n') {
      if (options.e) {
        printf("$");
      }
      printf("\n");
    } else if (options.v && ((c < 32 && c != '\n' && c != '\t') || c == 127)) {
      print_with_nonprint(c);
    } else {
      putchar(c);
    }
  }
}

int is_blank_line(const char *line, int length) {
  return length == 1 && line[0] == '\n';
}

// не понял что на маке работает потом 1 из 2 убрать
// void print_with_nonprint(unsigned char c) {
//   if (c < 32 && c != '\n' && c != '\t') {
//     printf("^%c", c + 64);
//   } else if (c == 127) {
//     printf("^?");
//   } else {
//     putchar(c);
//   }
// }

void print_with_nonprint(unsigned char c) {
  if (c < 32 && c != '\n' && c != '\t') {
    // Печатаем управляющие символы в виде ^X
    printf("^%c", c + 64);
  } else if (c == 127) {
    // Печатаем символ DEL как ^?
    printf("^?");
  } else if (c >= 128) {
    // Печатаем символы с установленным старшим битом как M-<символ>
    printf("M-");
    if (c >= 128 + 32 && c != 128 + 127) {
      printf("%c", c - 128);
    } else if (c == 128 + 127) {
      printf("^?");
    } else {
      printf("^%c", c - 128 + 64);
    }
  } else {
    // Печатаем остальные символы как есть
    putchar(c);
  }
}

int read_line(FILE *file, char *buffer, size_t size) {
  int count = 0;
  int c;
  while (count < (int)size && (c = fgetc(file)) != EOF) {
    buffer[count++] = (char)c;
    if (c == '\n') {
      break;
    }
  }
  if (count == 0 && c == EOF) {
    return -1;  // EOF достигнут, данных нет
  }
  return count;
}
