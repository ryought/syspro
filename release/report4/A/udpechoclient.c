#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> //strlen

void sendandwait(FILE *fp, int socket, const struct sockaddr *serveraddr, socklen_t serverlen) {
  int n;
  char sendline[1024], recvline[1025];

  while (fgets(sendline, 1024, fp)!=NULL) {
    sendto(socket, sendline, strlen(sendline), 0, serveraddr, serverlen);
    n = recvfrom(socket, recvline, 1024, 0, NULL, NULL);
    recvline[n] = 0;
    fputs(recvline, stdout);
  }
}

int main(int argc, char* argv[]) {
  if(argc != 3){
    printf("usage: udpechoclient (host IPv4) (port number)\n");
    return -1;
  }else{
    printf("host:%s, port:%s\n", argv[1], argv[2]);
  }
  
  int s;
  struct sockaddr_in serveraddr;

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(atoi(argv[2]));
  serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

  s = socket(AF_INET, SOCK_DGRAM, 0);

  sendandwait(stdin, s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  return 0;
}
