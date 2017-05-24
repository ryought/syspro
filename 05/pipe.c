#include <stdio.h>
#include <unistd.h>

int main() {
  int fd[2];
  pid_t pid;
  pipe(fd);

  if((pid=fork()) == 0){
    // child processが読み込んで出力する
    close(fd[1]);
    char buf[1024];

    // fd[0] readするがわ
    int i = read(fd[0], buf, 1024);
    printf("(%d) %s\n", i, buf);
    
  } else {
    // parent processはHelloをpipe経由で送信する
    close(fd[0]);
    char buf[1024] = "Hello";
    // fd[1] writeする口
    write(fd[1], buf, 6);
    
  }
  return 0;
}
