#include <stdio.h>
#include <unistd.h>

int main() {
  int foo, pid;

  pid = fork();

  if(pid == 0){
    foo = 9;
  } else if(pid > 0){
    foo = 2;
  }

  printf("foo=%d\n", foo);
  return 0;
  
}
