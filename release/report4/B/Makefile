all: server client
CC = gcc
CFLAGS = -Wall

server_thr: tcpechoserver1_thread.c
	$(CC) $< -o $@
server_sel: tcpechoserver2_select.c
	$(CC) $< -o $@
client: tcpechoclient.c
	$(CC) $< -o $@

run-client:
	./client 127.0.0.1 1255

run-server-thr:
	./server_thr 1255
run-server-sel:
	./server_sel 1255

clean:
	rm -f server_sel client server_thr
