// iperfc (host) (port num)
// スループット測定ツール クライアント側
// TCPで単方向通信

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
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
  if(argc != 4){
     printf("usage: iperfc (host IPv4) (port number) (datasize)\n");
    return -1;
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

  //データの準備
  char send[1024], recv[1025];
  for(int k=0; k<1024; k++){
    send[k] = rand()%92+33; //アスキーコードの文字の範囲の乱数を発生
  }
  
  int n;
  int i;
  int datasize = atoi(argv[3]);  // 送信データ数

  // まず送信データ数を送る
  write(s, &datasize, sizeof(datasize));
  // その確認
  n = read(s, recv, 1024);
  recv[n] = '\0';
  if(n > 0){
    if(recv[n-1]!='C'){
      printf("fail\n");
      return -1;
    }
  }


  // 確認取れたら送信作業に移る

  struct timeval start, end;
  gettimeofday(&start, NULL);

  // 重いデータの送信
  for(i = 0; i < (datasize/1024)+1; i++){
    write(s, send, strlen(send));
  }
  
  // 終了のエコーバックを待つ
  n = read(s, recv, 1024);
  recv[n] = '\0';
  
  if(n > 0){
    if(recv[n-1]!='C'){
      printf("fail\n");
      return -1;
    }
  }
  
  gettimeofday(&end, NULL);

  int time = (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec);
  int throughput = datasize * 8 / time;

  printf("%d\t%d\t%d", datasize, time, throughput);
  return 0;
}



