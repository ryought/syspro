all: udpechoserver udpechoclient
CC = gcc
CFLAGS = -Walla

udpechoserver: udpechoserver2.c
	$(CC) $< -o $@
udpechoclient: udpechoclient2.c
	$(CC) $< -o $@

clean:
	rm -f udpechoserver udpechoclient
