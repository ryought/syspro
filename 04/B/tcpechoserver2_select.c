// selectで対応するバージョン

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> //strlen

#include <pthread.h>

#include <errno.h>
#include <unistd.h>

// スレッド数 = 同時接続数
#define NSOCKET 5



// sockfdに対してエコーバックする
void str_echo(int sockfd) {
  ssize_t n;
  char buf[1024];
 again:
  while((n = read(sockfd, buf, 1024)) > 0) {
    write(sockfd, buf, n);
    printf("wrote %zd byte\n", n);
  }
  if(n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    printf("read error");
}




int main(int argc, char* argv[]) {
  if(argc != 2){
     printf("usage: tcpclient (port number)\n");
    return -1;
  }else{
    printf("port:%s\n", argv[1]);
  }
  
  int s;
  s = socket(AF_INET, SOCK_STREAM, 0);
  
  struct sockaddr_in serveraddr, clientaddr;
  socklen_t clientaddrlen = sizeof(clientaddr);
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(atoi(argv[1]));
  //serveraddr.sin_port = htons(1234);

  bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  listen(s, 16);  // int backlog 処理されていない接続要求がたまるキューの大きさ

  // selectのfile descriptorの集合
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(s, &fds);              /* accept待ち受けようsocketを追加 */

  // selectの待ち時間 5秒
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  
  int ns; // 新しいsocket(accept後)
  int i;  // インデックス
  
  while(1) {
    // accept待ち
    if(select(FD_SETSIZE, &fds, NULL, NULL, &tv) < 0)
      printf("error\n");

    // connectedのsocketをpoll
    for (i=0; i<FD_SETSIZE; i++) {
      if(FD_ISSET(i, &fds)){ // 読み出し可能ソケットがあった！
        printf("readable %d\n", i);
        
        if(i == s){ // accept待ちのソケット
          // accept要求が来た acceptしたあと、新しい監視対象ソケットをつくる
          printf("accepting\n");
          ns = accept(i, (struct sockaddr *)&clientaddr, &clientaddrlen);
          FD_SET(ns, &fds);
        }else{
          printf("reading\n");     
          // それ以外のところ 普通にリードする
          str_echo(i);
        }
        
      }
    }
  }
  
  return 0;
}
