#include "s21_grep.h"

int main(int argc, char** argv) {
  options options = parseOptions(argc, argv);
  regex_t regex;

  if (regcomp(&regex, options.pattern, REG_EXTENDED | options.i) != 0) {
    perror("Error of compiling regular expression");
    return 1;
  }

  for (int i = optind; i < argc; i++) {
    processFile(options, argv[i], &regex);
  }

  regfree(&regex);
  return 0;
}

void processFile(options options, const char* filePath, regex_t* regex) {
  FILE* file = fopen(filePath, "r");

  if (!file) {
    if (!options.s) {
      perror(filePath);
    }
    return;
  }

  char lineBuffer[4096];
  int lineCount = 1;
  int matchCount = 0;

  while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
    size_t len = strlen(lineBuffer);
    if (len > 0 && lineBuffer[len - 1] == '\n') {
      lineBuffer[len - 1] = '\0';
    }

    int matchResult = regexec(regex, lineBuffer, 0, NULL, 0);

    if ((matchResult == 0 && !options.v) || (options.v && matchResult != 0)) {
      if (!options.c && !options.l) {
        if (!options.h) {
          printf("%s:", filePath);
        }
        if (options.n) {
          printf("%d:", lineCount);
        }

        if (options.o) {
          printMatches(regex, lineBuffer);
        } else {
          printf("%s\n", lineBuffer);
        }
      }
      matchCount++;
    }

    lineCount++;
  }

  if (options.c && !options.l) {
    if (!options.h) {
      printf("%s:", filePath);
    }
    printf("%d\n", matchCount);
  }

  if (options.l && matchCount > 0) {
    printf("%s\n", filePath);
  }

  fclose(file);
}

void processFilePatterns(options* options, const char* filepath) {
  FILE* file = fopen(filepath, "r");

  if (!file) {
    if (!options->s) {
      perror(filepath);
    }
    exit(EXIT_FAILURE);
  }

  char lineBuffer[4096];

  while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
    size_t len = strlen(lineBuffer);
    if (len > 0 && lineBuffer[len - 1] == '\n') {
      lineBuffer[len - 1] = '\0';
    }

    mergePatterns(options, lineBuffer);
  }

  fclose(file);
}

void printMatches(regex_t* regex, const char* string) {
  regmatch_t matchInfo;

  size_t offset = 0;
  // пока в нашей строке есть хотя бы 1 свопадение ...
  while (regexec(regex, string + offset, 1, &matchInfo, 0) == 0) {
    // мы бежим от rm_so: Начальная позиция совпадения до конца и выводим
    // совпадение
    for (int i = matchInfo.rm_so; i < matchInfo.rm_eo; i++) {
      putchar(string[offset + i]);
    }
    putchar('\n');
    // обработка крайнего случая, когда мы зацикливаемся на пустой строке
    if (matchInfo.rm_eo > 0) {
      offset += matchInfo.rm_eo;
    } else {
      offset += 1;
    }
  }
}

void mergePatterns(options* options, const char* pattern) {
  // добавляем | в конец 1 searchString aka pattern'a если уже есть хотя бы 1
  // паттерн
  if (options->patternLen > 0) {
    options->pattern[options->patternLen] = '|';
    options->patternLen++;
  }

  // добавление нового шаблона pattern в строку options->pattern
  // аргументы snprintf 1) куда записать 2) maxlen 3) формат записи нового
  // шаблона (в скобках, чтобы он воспринимал их как опции) 4) сам шаблон
  snprintf(options->pattern + options->patternLen,
           sizeof(options->pattern) - options->patternLen, "(%s)", pattern);
  // обновляем длину нашей searchString с учетом скобок
  options->patternLen += strlen(pattern) + 2;
}

options parseOptions(int argc, char* argv[]) {
  options options = {0};
  int option;
  optionsParsing(&options, &option, argc, argv);

  // ловим паттерн если не указан флаг -е
  if (!options.patternLen && optind < argc)
    mergePatterns(&options, argv[optind++]);

  // если у нас только 1 файл, автоматически ставим флаг h (не выводим filename)
  if (argc - optind == 1) options.h = 1;

  return options;
}

void optionsParsing(options* options, int* option, int argc, char** argv) {
  while ((*option = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (*option) {
      case 'e': {
        options->e = 1;
        mergePatterns(options, optarg);
        break;
      }
      case 's': {
        options->s = 1;
        break;
      }
      case 'i': {
        options->i = REG_ICASE;
        break;
      }
      case 'v': {
        options->v = 1;
        break;
      }
      case 'c': {
        options->c = 1;
        break;
      }
      case 'l': {
        options->l = 1;
        break;
      }
      case 'n': {
        options->n = 1;
        break;
      }
      case 'h': {
        options->h = 1;
        break;
      }
      case 'f': {
        options->f = 1;
        processFilePatterns(options, optarg);
        break;
      }
      case 'o': {
        options->o = 1;
        break;
      }
    }
  }
}