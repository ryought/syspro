#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> //strlen

void str_echo(int sockfd) {
  ssize_t n;
  char buf[1024];
 again:
  while((n = read(sockfd, buf, 1024)) > 0)
    write(sockfd, buf, n);
  if(n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    printf("read error");
}


int main(int argc, char* argv[]) {
  if(argc != 3){
     printf("usage: udpechoclient (host IPv4) (port number)\n");
    return -1;
  }else{
    printf("host:%s, port:%s\n", argv[1], argv[2]);
  }
  
  int listenfd, connfd;
  struct sockaddr_in serveraddr, clientaddr;
  socklen_t clilen;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(atoi(argv[2]));

  bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  listen(listenfd, LISTENQ);

  for(;;){
    clilen = sizeof(clientaddr);
    connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clilen);

    if ((childpid=fork()) == 0) {
      close(listenfd);
      str_echo(connfd);
      exit(0);
    }
    close(connfd);
  }

  return 0;
}
