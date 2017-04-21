/* 課題B cpコマンドの実装  */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  int fd, fdo;
  char buf[512];

  fd = open("a.txt", O_RDONLY);
  fdo = open("b.txt", O_WRONLY|O_CREAT);

  while (read(fd, buf, 36) > 0) {
    printf("%s", buf);
  }
  close(fd);
}
