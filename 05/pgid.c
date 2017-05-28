#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {
  pid_t pid;
  int p1[2], p2[2];
  char c='?';

  if (pipe(p1) != 0)
    perror("pipe() #1 error");
  else if (pipe(p2) != 0)
    perror("pipe() #2 error");
  else
    if ((pid = fork()) == 0) {
      printf("child's process group id is %d\n", (int) getpgrp());
      write(p2[1], &c, 1);
      puts("child is waiting for parent to complete task");
      read(p1[0], &c, 1);
      printf("child's process group id is now %d\n", (int) getpgrp());
      return 0;
    }
    else {
      printf("parent's process group id is %d\n", (int) getpgrp());
      read(p2[0], &c, 1);
      printf("parent is performing setpgid() on pid %d\n", (int) pid);

      setpgid(pid, 0);
      
      write(p1[1], &c, 1);
      printf("parent's process group id is now %d\n", (int) getpgrp());
      return 0;
    }
}
