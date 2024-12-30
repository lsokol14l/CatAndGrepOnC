#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Manual

  Весь таск завязан в основном на библиотеке getopt, а именно ее методе
  getopt_long и ее свойствах по типу struct option

struct option {
  const char *name; // название длинной опции, например (--number)
  int has_arg; // переменная содержит число от 0 до 2, каждое из которых
               // обозначает: no_argument requied_argument optional_argument
               // соотвественно
  int *flag; // флаг, отвечает за запись результата функции в переменную (нам не
             //понадобится)
  int val; // символ на который мы условно заменяем название длинной опции
}

Аргументы getopt_long (5):
  (1)argc и (2)argv, (3)optsting - строка с короткими опциями "benst",
  (4)longopts - массив struct option, для длинных опций, все поля расписаны выше
  (5)longindex - Механизм определения индекса отключается, если NULL и функция
просто обрабатывает опции

функция возвращает код опции (буква), 0 в случае, если
длинная опция, -1, когда все опции обработаны, '?' при ошибке

*/

typedef struct options {
  bool b;
  bool n;
  bool s;
  bool E;
  bool T;
  bool v;
} opt;

void consoleLogMode();
int getLine(char **lineptr, size_t *n, FILE *stream);
void putCharVFlag(unsigned char symbol);
int isLineBreak(int symbol);
void openAndOutputFile(char **argv, opt options);
void outputString(opt options, char *string, int strLen, int *strCounter);
opt parse_options(int argc, char **argv, struct option *longopts);

#endif