/* 課題B cpコマンドの実装  */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  int fd = open("out.txt", O_RDWR|O_CREAT|O_APPEND, 0666);
  printf("%d\n", fd);
  if (fd < 0) {
    return -1;
  }

  if (write(fd, "This will be output to testfile.txt\n", 36) != 36) {
    return -1;
  }

  return 0;
}
