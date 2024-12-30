SHELL:= /bin/bash
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror
BUILD=./
FILENAME=s21_cat
FILE=test.txt

s21_cat: clean s21_cat.o
	$(CC) $(CFLAGS) s21_cat.o -o $(BUILD)/$(FILENAME)
s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c	
tests:
	-diff <(./s21_cat $(FILE)) <(cat $(FILE))
	-diff <(./s21_cat -b $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat -e $(FILE)) <(cat -e $(FILE))
	-diff <(./s21_cat -n $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat -s $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat -t $(FILE)) <(cat -t $(FILE))	
	-diff <(./s21_cat -E -v $(FILE)) <(cat -e $(FILE))	
	-diff <(./s21_cat -T -v $(FILE)) <(cat -t $(FILE))
	-diff <(./s21_cat --number-nonblank $(FILE)) <(cat -b $(FILE))
	-diff <(./s21_cat --number $(FILE)) <(cat -n $(FILE))
	-diff <(./s21_cat --squeeze-blank $(FILE)) <(cat -s $(FILE))
	-diff <(./s21_cat -v $(FILE)) <(cat -v $(FILE))
	-diff <(./s21_cat -n $(FILE) -t) <(cat -n $(FILE) -t)
	-diff <(./s21_cat -benst $(FILE)) <(cat -benst $(FILE))
	-diff <(./s21_cat -bn $(FILE)) <(cat -bn $(FILE))
	-diff <(./s21_cat -nb $(FILE)) <(cat -nb $(FILE))
clean:
	rm -rf *.o $(BUILD)/$(FILENAME)
rebuild: clean s21_cat

debug:
	cd ./build/Debug && ./outDebug -n test.txt

