all: shell
CC = gcc
CFLAGS = -Wall

shell.o: shell.c
	${CC} -c $<

parse.o: parser/parse.c parser/parse.h
	${CC} -c $<

shell: shell.o parse.o
	${CC} shell.o parse.o -o $@

exec_prog: exec_prog.c
	$(CC) $< -o $@

clean:
	rm -f shell parse.o shell.o
