all: server client
CC = gcc
CFLAGS = -Wall

server_thr: tcpechoserver1_thread.c
	$(CC) $< -o $@
server: tcpechoserver2_select.c
	$(CC) $< -o $@
client: tcpechoclient.c
	$(CC) $< -o $@

run-client:
	./client 127.0.0.1 1255

run-server:
	./server 1255

clean:
	rm -f server client server_thr
