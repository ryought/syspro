/* 課題B cpコマンドの実装  */
// DONE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  if (argc != 3) {
    // not enough args
    return -1;
  }
  int fd0, fd1; 
  int i, j;
  int BUFSIZE = 512;
  char buf[BUFSIZE];
  

  fd0 = open(argv[1], O_RDONLY);
  if(fd0 < 0) {
    // error
    perror(argv[1]);
    return -1;
  }
  
  fd1 = open(argv[2], O_WRONLY|O_CREAT|O_EXCL, 0666);
  if(fd1 < 0) {
    // error
    perror(argv[2]);
    return -1;
  }

  // TODO 同じファイルにリンクしていることを示す
    
  while((i=read(fd0, buf, BUFSIZE)) > 0) {
    j = write(fd1, buf, i);
    if(j < 0){
      // 書き込みエラー
      perror(NULL);
      return -1;
    }else if(j < i){
      // 全部は書き込めてない
      // TODO ここの処理
      // memmove(buf+j, buf, i-j);
    }
    printf("%d, %d\n", i, j);
  }

  if(i < 0){
    perror(NULL);
    return -1;
  }

  close(fd0); close(fd1);

  return 0;
}
