#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  int fd;
  char buf[512];
  int i;

  fd = open("a.txt", O_RDONLY);

  do {
    i=read(fd, buf, 512);
    printf("i:%d %s\n", i, buf);
  } while (i > 0);
  
  close(fd);

  
  return 0;
}
