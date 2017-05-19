#include <stdio.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>


void echo(int socket, struct sockaddr *clientaddr, socklen_t clen) {
  int n;
  socklen_t len;
  char buf[1024];

  for(;;){
    len = clen;
    n = recvfrom(socket, buf, 1024, 0, clientaddr, &len);
    sendto(socket, buf, n, 0, clientaddr, len);
  }
}


int main() {
  int s;
  struct sockaddr_in serveraddr, clientaddr;
  s = socket(AF_INET, SOCK_DGRAM, 0);
  bzero(&serveraddr, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(1234);

  bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  echo(s, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
}
