all: iperfs iperfc
CC = gcc
CFLAGS = -Wall

iperfs: iperfs.c
	$(CC) $< -o $@
iperfc: iperfc.c
	$(CC) $< -o $@

clean:
	rm -f iperfs iperfc
