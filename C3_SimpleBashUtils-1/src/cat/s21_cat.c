#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc == 1) {
    consoleLogMode();
  } else {
    struct option longopts[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {0, 0, 0, 0}};  // массив должен завершаться элементом с нулями
    opt options = parse_options(argc, argv, longopts);
    openAndOutputFile(argv, options);
  }
  return 0;
}

opt parse_options(int argc, char **argv, struct option *longopts) {
  opt options = {0};
  int option;
  while ((option = getopt_long(argc, argv, "benstTEv", longopts, 0)) != -1) {
    switch (option) {
      case 'b': {
        options.b = true;
        break;
      }
      case 'e': {
        options.E = true;
        options.v = true;
        break;
      }
      case 'n': {
        options.n = true;
        break;
      }
      case 's': {
        options.s = true;
        break;
      }
      case 't': {
        options.T = true;
        options.v = true;
        break;
      }
      case 'T': {
        options.T = true;
        break;
      }
      case 'E': {
        options.E = true;
        break;
      }
      case 'v': {
        options.v = true;
        break;
      }
      case '?': {
        fprintf(stderr,
                "Usage : ./s21_cat <flags> <filename1> <filename2>...\n");
        exit(1);
        break;
      }
    }
  }
  if (options.b) options.n = 0;
  return options;
}

void outputString(opt options, char *string, int strLen, int *strCounter) {
  // вывод номера строк перед выводом строки соотвественно b > n
  if (options.n) printf("%6d\t", *strCounter);

  if (options.b && string[0] != '\n')
    printf("%6d\t", *strCounter);
  else if (options.b && string[0] == '\n')
    (*strCounter)--;

  // посимвольный вывод строки с изменениями в зависимости от флагов
  for (int i = 0; i < strLen; i++) {
    if (options.T && string[i] == '\t') {
      printf("^I");
      i++;
    }

    if (options.E && string[i] == '\n') putchar('$');

    if (options.v)
      putCharVFlag(string[i]);
    else
      putchar(string[i]);
  }
}

void putCharVFlag(unsigned char c) {
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

void openAndOutputFile(char **argv, opt options) {
  FILE *file = fopen(argv[optind], "r");
  if (!file) {
    fprintf(stderr, "Not able to open the file.\n");
    exit(1);
  }

  char *string = NULL;
  size_t strLen = 0;
  int readStatus = 0;
  int strCounter = 1;
  int emptyStringCounter = 0;

  while ((readStatus = getLine(&string, &strLen, file)) != -1) {
    if (string[0] == '\n')
      emptyStringCounter++;
    else
      emptyStringCounter = 0;

    if (!options.s || emptyStringCounter <= 1) {
      outputString(options, string, readStatus, &strCounter);
      strCounter++;
    }
  }
  free(string);
  fclose(file);
}

// знаковый эквивалент size_t.
int getLine(char **lineptr, size_t *n, FILE *stream) {
  if (lineptr == NULL || n == NULL) {
    return -1;
  }

  if (*lineptr == NULL || *n == 0) {
    *n = 128;
    *lineptr = (char *)malloc(*n);
    if (*lineptr == NULL) {
      return -1;
    }
  }

  char *buffer = *lineptr;
  size_t size = *n;
  int pos = 0;
  int c;

  while ((c = fgetc(stream)) != EOF) {
    // Если буфер недостаточно велик, увеличиваем его
    if (pos + 1 >= (int)size) {
      size_t new_size = size * 2;  // Удваиваем размер
      char *new_buffer = (char *)realloc(buffer, new_size);
      if (new_buffer == NULL) {
        return -1;  // Ошибка выделения памяти
      }
      buffer = new_buffer;
      size = new_size;
    }

    buffer[pos++] = (char)c;

    if (c == '\n') {
      break;
    }
  }

  // Проверка на пустой файл
  if (pos == 0 && c == EOF) {
    return -1;
  }

  buffer[pos] = '\0';

  // Обновляем указатели и размер
  *lineptr = buffer;
  *n = size;

  return pos;  // Возвращаем количество считанных символов
}

void consoleLogMode() {
  size_t n = 0;
  char *buffer;
  int readStatus;

  while ((readStatus = getLine(&buffer, &n, stdin)) != -1) {
    printf("%s", buffer);
  }
}