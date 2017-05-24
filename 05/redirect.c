// pipeとdup で 標準出力、標準入力 をredirectするサンプル
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "parser/parse.h" // パーサ
 
int main(int argc, char* argv[], char *envp[]) {
  int fd[2];
  pipe(fd);

  int in = open("input", O_RDONLY);
  dup2(in, 0);
  close(in);

  
  int out = open("output", O_WRONLY|O_CREAT,0666);
  dup2(out, 1);
  close(out);

  execve("/usr/bin/wc", NULL, NULL);
  return 0;
}
