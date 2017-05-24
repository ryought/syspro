#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> //strlen
#include <unistd.h>

void client(FILE *fp, int sockfd) {
  char send[1024], recv[1025];
  int n;
  while(fgets(send, 1024, fp) != NULL) {
    write(sockfd, send, strlen(send));
    if((n = read(sockfd, recv, 1024)) == 0)
      printf("terminated\n");
    recv[n] = '\0';
    printf("read %d byte\n", n);
    fputs(recv, stdout);
  }
}


int main(int argc, char* argv[]) {
  if(argc != 3){
     printf("usage: tcpechoclient (host IPv4) (port number)\n");
    return -1;
  }else{
    printf("host:%s, port:%s\n", argv[1], argv[2]);
  }
  
  int s;
  struct sockaddr_in serveraddr;

  s = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(atoi(argv[2]));
  //serveraddr.sin_addr.s_addr = htonl();
  inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);

  connect(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  client(stdin, s);

  return 0;
}
