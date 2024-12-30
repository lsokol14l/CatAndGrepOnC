#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

/*
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

#define CAT_OPTIONS_MANUAL                                                     \
  "  -b (GNU: --number-nonblank)                             number nonempty " \
  "output lines\n"                                                             \
  "  -e (GNU only: -E the same, but without implying -v)     display $ at "    \
  "end "                                                                       \
  "of each line\n"                                                             \
  "  -n (GNU: --number)                                      number all "      \
  "output "                                                                    \
  "lines\n"                                                                    \
  "  -s (GNU: --squeeze-blank)                               squeeze "         \
  "multiple "                                                                  \
  "adjacent blank lines\n"                                                     \
  "  -t (GNU: -T the same, but without implying -v)          display TAB "     \
  "characters "                                                                \
  "as ^I\n"                                                                    \
  "  -v                                                          show "        \
  "non-printing characters\n"

typedef struct {
  int b;  // number-nonblank
  int e;  // show ends
  int n;  // number all lines
  int s;  // squeeze blank
  int t;  // show tabs
  int v;  // show non-printing
} options_t;

void process_line(char *line, int len, options_t options);
int is_blank_line(const char *line, int length);
void print_with_nonprint(unsigned char c);
int parse_options(int argc, char *argv[], options_t *options);
void process_file(const char *filename, options_t options,
                  int *line_number_ptr);
int read_line(FILE *file, char *buffer, size_t size);

#endif  // S21_CAT_H
