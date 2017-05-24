// dup2()を使って、puts(普通は標準出力に出す)の結果をテキストファイルに書き出す
#include <stdio.h>
#include <fcntl.h>  // open
#include <unistd.h> // dup2

int main() {
  int fd = open("test", O_RDWR|O_CREAT|O_APPEND, 0666);
  dup2(fd, 1);

  puts("test");
  return 0;
}
