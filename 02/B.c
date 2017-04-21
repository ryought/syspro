/* 課題B cpコマンドの実装  */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  int fd, fdo;
  char buf[512];
  int i;
  
  fd = open("a.txt", O_RDONLY);
  fdo = open("b.txt", O_WRONLY|O_CREAT);

  do {
    i = read(fd, buf, 512);
    printf("result: %zd\n", write(fdo, buf, i));
    printf("%d: %s",i ,  buf);
  } while (i > 0);
  close(fd);
  close(fdo);
  return 0;
}
