#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* MANUAL

-e Позволяет указать шаблон для поиска. Этот флаг полезен, если нужно указать
несколько шаблонов. Каждый следующий шаблон указывается через отдельный флаг -e.

grep -e "error" -e "warning" file.txt

-i делает поиск нечувствительным к регистру

-v инвертирует результат поиска, выводя строки, которые не содержат заданного
шаблона.

-c выводит только количество строк, соответствующих шаблону (без вывода самих
строк)

-n выводит номер строки, где был найден шаблон, вместе с самой строкой

-l выводит только имена файлов, в которых был найден шаблон (без вывода самих
строк)

-h При поиске в нескольких файлах не выводит имя файла перед совпадением.

-s Подавляет ошибки, например, при попытке прочитать файл, к которому нет
доступа.

grep -s "error" non_existent_file.txt

-f позволяет указать файл, содержащий шаблоны для поиска. (каждый шаблон
в файле должен быть указан на новой строке)

grep -f patterns.txt file.txt

-o флаг -o выводит только найденные совпадения(Именно совпадающую часть слова, а
не все слово), а не всю строку целиком.


regexec() аргументы: (вернет 0 в случае свопадения и не NULL в проивном случае)

  preg: Указатель на скомпилированное регулярное выражение (объект типа
regex_t), который был создан с помощью regcomp.

  string: Строка, которую нужно проверить на соответствие регулярному выражению.

  nmatch: Максимальное количество совпадений, которые нужно сохранить в массиве
pmatch.

  pmatch: Массив структур regmatch_t, в котором будут сохранены результаты
совпадений. Каждая структура regmatch_t содержит два поля:

    rm_so: Начальная позиция совпадения.
    rm_eo: Конечная позиция совпадения.
  eflags: Флаги, которые изменяют поведение функции regexec. Возможные значения:


*/

typedef struct {
  char pattern[4096];
  int patternLen, e, s, i, v, c, l, n, h, f, o;
} options;

options parseOptions(int argc, char** argv);
void processFile(options arg, const char* filePath, regex_t* regex);
void mergePatterns(options* arg, const char* pattern);
void processFilePatterns(options* arg, const char* filePath);
void printMatches(regex_t* regex, const char* string);
void optionsParsing(options* options, int* option, int argc, char** argv);

#endif
