/* 課題D wcコマンドの実装  */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

// ./a.out hoge.txt  ->  行数、単語数、バイト数
int main(int argc, char *argv[]){
  if (argc != 2) {
    // not enough args
    return -1;
  }
  
  int fd; 
  int i;
  int bytes;
  int lines;
  int words;
  char buf[512];

  fd = open(argv[1], O_RDONLY);
  if(fd < 0) {
    // error
    perror(argv[1]);
    return -1;
  }
    
  while((i=read(fd, buf, 512)) > 0) {
    // bytes
    bytes += i;                 /* バイト数は読んだバイト数そのまま足す */
    
    for (int k=0; k<i; k++) {
      // lines
      if (buf[k]=='\n') {
        lines += 1;
      }
    }
    for (int k=0; k<i-1; k++) {
      // words  文字の一つ前にあるスペースをカウント
        if (isspace(buf[k]) && !isspace(buf[k+1])) {
          // TODO 先頭のスペースが数えられてしまうよ
          printf("found %c, %c\n", buf[k], buf[k+1]);
          words += 1;
        }
    }

    
    printf("%d\n", i);
  }

  if(i < 0){
    perror(NULL);
    return -1;
  }


  printf("%d %d %d\n", lines, words, bytes);

  close(fd);

  return 0;
}
