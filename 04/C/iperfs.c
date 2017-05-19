// threadで監視するバージョン

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> //strlen

#include <pthread.h>

#include <errno.h>
#include <unistd.h>

// スレッド数 = 同時接続数
#define NTHREAD 5

// テストデータのバイト数
#define DATASIZE 1000000

// スレッドに渡す引数の構造体
struct thread_arg {
  int i;
  int socket;
};


// テストデータを全部受け取って、終端文字があったらそれで終わりと判断しCを送る
void recvandcheck(int sockfd) {
  printf("str_echo\n");
  ssize_t n;
  char buf[1024];
  int total = 0;
 again:
  while((n = read(sockfd, buf, 1024)) > 0) {
    printf("received %zd(total %d)\n", n, total);
    total = total + n;
    if(total>DATASIZE)
      break;
  }
  if(n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    printf("read error");
  if(total > DATASIZE){
    printf("read alldata\n");
    buf[0] = 'C';
    write(sockfd, buf, 1);
    printf("finishcode sent\n");
    return;
  }
}


// 各スレッドが実行するコード
void *server(void *arg) {
  struct thread_arg *thread_arg = (struct thread_arg *)arg;
  int s = thread_arg->socket;
  int i = thread_arg->i;
  printf("thread #%d : %d\n",i ,s);
  int ss;
  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  
  while(1) {
    // クライアントと接続し、仕事して、クローズまでする      
    ss = accept(s, (struct sockaddr *)&addr, &addrlen);
    printf("[#%d] accepted\n", i);
    recvandcheck(ss); // read, then write
    close(ss);
  }
  
  pthread_exit(NULL);
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
  
  struct sockaddr_in serveraddr;
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(atoi(argv[1]));
  //serveraddr.sin_port = htons(1234);

  bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

  listen(s, 16);  // int backlog 処理されていない接続要求がたまるキューの大きさ

  int i;
  pthread_t thread[NTHREAD];
  struct thread_arg arg[NTHREAD];
  printf("creating thread\n");
  for(i=0; i<NTHREAD; ++i){
    arg[i].i = i;
    arg[i].socket = s;
    pthread_create(&thread[i], NULL, server, &arg[i]);
  }
  for(i=0; i<NTHREAD; ++i)
    pthread_join(thread[i], NULL);
  
  return 0;
}
