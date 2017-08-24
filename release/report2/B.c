/* 課題B cpコマンドの実装  */
// DONE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  if (argc != 3) {
    // not enough args
    printf("not enough arguments\n");
    return -1;
  }
  struct timeval start, end;
  int fd0, fd1; 
  int i, j, total;
  char *p;
  int BUFSIZE = 8192;
  char buf[BUFSIZE];
  
  gettimeofday(&start, NULL);

  fd0 = open(argv[1], O_RDONLY);
  if(fd0 < 0) {
    // error
    perror(argv[1]);
    return -1;
  }
  
  fd1 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
  if(fd1 < 0) {
    // error
    perror(argv[2]);
    return -1;
  }

  // i:読み込んだバイト数 j:書き込んだバイト数
  while((i=read(fd0, buf, BUFSIZE)) > 0) {
    // p:バッファの中で書き込みが終わっていない領域の先頭へのポインタ
    j = 0;
    p = buf;
    while(i - j > 0){
      j = write(fd1, p, i);
      //printf("i:%d,j:%d\n", i, j);
      if(j<0) {
        perror(NULL);
        return -1;
      }
      p = p + j;
      i = i - j;
    }
    printf("%d, %d\n", i, j);
  }

  if(i < 0){
    perror(NULL);
    return -1;
  }

  close(fd0); close(fd1);

  gettimeofday(&end, NULL);
  printf("%ld %d\n", start.tv_sec, start.tv_usec);
  printf("%ld %d\n", end.tv_sec, end.tv_usec);
  printf("%ld\n", 
    (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));

  return 0;
}
