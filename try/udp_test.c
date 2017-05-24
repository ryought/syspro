#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
  int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in addr;
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = INADDR_ANY;
  
  bind(s, (struct sockaddr *)&addr, sizeof(addr));

  char buf[512];
  memset(buf, 0, sizeof(buf));

  //struct sockaddr_from from;
  
  recv(s, buf, sizeof(buf), 0);

  printf("%s\n", buf);

  close(s);

  return 0;
}
