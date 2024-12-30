SHELL:= /bin/bash
CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror
BUILD=./
FILENAME=s21_grep
FILE=*.txt
PATTERNS=patterns.txt

s21_grep: clean s21_grep.o
	$(CC) $(CFLAGS) s21_grep.o -o $(BUILD)/$(FILENAME)
s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c	
tests:
	-diff <(./s21_grep He $(FILE)) <(grep He $(FILE))
	-diff <(./s21_grep -v No $(FILE)) <(grep -v No $(FILE))
	-diff <(./s21_grep -i He $(FILE)) <(grep -i He $(FILE))
	-diff <(./s21_grep -n He $(FILE)) <(grep -n He $(FILE))
	-diff <(./s21_grep -l He $(FILE)) <(grep -l He $(FILE))
	-diff <(./s21_grep -iv He $(FILE)) <(grep -iv He $(FILE))
	-diff <(./s21_grep -in He $(FILE)) <(grep -in He $(FILE))
	-diff <(./s21_grep -ie "^he" $(FILE)) <(grep -ie "^he" $(FILE))
	-diff <(./s21_grep -ince "^he" $(FILE)) <(grep -ince "^he" $(FILE))
	-diff <(./s21_grep -s he badfile) <(grep -s he badfile)
	-diff <(./s21_grep -e He -e he $(FILE)) <(grep -e He -e he $(FILE))	
	-diff <(./s21_grep -f $(PATTERNS) $(FILE)) <(grep -f $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oi he $(FILE)) <(grep -oi he $(FILE))
	-diff <(./s21_grep -ohf $(PATTERNS) $(FILE)) <(grep -ohf $(PATTERNS) $(FILE))
	-diff <(./s21_grep -oinc he $(FILE)) <(grep -oinc he $(FILE))
	-diff <(./s21_grep -iv he $(FILE)) <(grep -iv he $(FILE))
	-diff <(./s21_grep -in he $(FILE)) <(grep -in he $(FILE))
	-diff <(./s21_grep -of $(PATTERNS) $(FILE)) <(grep -of $(PATTERNS) $(FILE))
	-diff <(./s21_grep -o He $(FILE)) <(grep -o He $(FILE))

clean:
	rm -rf *.o $(BUILD)/$(FILENAME)
rebuild: clean s21_grep

debug:
	cd ./build/Debug && ./outDebug -n test.txt

