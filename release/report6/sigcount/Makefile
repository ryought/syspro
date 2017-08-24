all: sigcount
CC = gcc
CFLAGS = -Wall

# 課題A 最適化オプションを使う
sigcount: sigcount.c
	$(CC) $< -o $@ -O3


clean:
	rm -f sigcount
